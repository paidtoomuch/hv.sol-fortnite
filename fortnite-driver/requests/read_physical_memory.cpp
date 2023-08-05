#pragma once

namespace request
{

	NTSTATUS resolve_dtb(invoke_data* request)
	{
		dtb_invoke data = { 0 };

		if (!modules::safe_copy(&data, request->data, sizeof(dtb_invoke)))
		{
			return STATUS_UNSUCCESSFUL;
		}

		PEPROCESS process = 0;
		if (imports::ps_lookup_process_by_process_id((HANDLE)data.pid, &process) != STATUS_SUCCESS)
		{
			printf("invalid process.\n");
			return STATUS_UNSUCCESSFUL;
		}

		physical::m_stored_dtb = pml4::dirbase_from_base_address( (void*) imports::ps_get_process_section_base_address( process ) );

		printf( "cr3: %llx\n", physical::m_stored_dtb );

		imports::obf_dereference_object( process );

		reinterpret_cast<dtb_invoke*> (request->data)->operation = true;

		return STATUS_SUCCESS;
	}

	NTSTATUS read_physical_memory(invoke_data* request)
	{
		read_invoke data = { 0 };

		if (!modules::safe_copy(&data, request->data, sizeof(read_invoke)))
		{
			return STATUS_UNSUCCESSFUL;
		}

		if ( !data.address || !data.pid || !data.buffer || !data.size || data.address >= 0x7FFFFFFFFFFF || !physical::m_stored_dtb )
			return STATUS_UNSUCCESSFUL;

		auto physical_address = physical::translate_linear(
			physical::m_stored_dtb,
			data.address);

		if (!physical_address)
		{
			return STATUS_UNSUCCESSFUL;
		}

		auto final_size = physical::find_min(
			PAGE_SIZE - (physical_address & 0xFFF),
			data.size);

		size_t bytes = 0;
		if (!NT_SUCCESS(physical::read_physical(
			physical_address,
			reinterpret_cast<void*>((reinterpret_cast<ULONG64>(data.buffer))),
			final_size,
			&bytes)))
		{
			printf( "failed to do read\n" );
			return STATUS_UNSUCCESSFUL;
		}

		return STATUS_SUCCESS;
	}


}