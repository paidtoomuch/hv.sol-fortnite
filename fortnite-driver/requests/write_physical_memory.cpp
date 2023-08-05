#pragma once

namespace request
{
	NTSTATUS write_physical_memory( invoke_data* request ) {
		write_invoke data = { 0 };

		if ( !modules::safe_copy( &data, request->data, sizeof( write_invoke ) ) ) {
			return STATUS_UNSUCCESSFUL;
		}

		if ( !data.address || !data.pid || !data.buffer || !data.size || data.address >= 0x7FFFFFFFFFFF || !physical::m_stored_dtb ) {
			return STATUS_UNSUCCESSFUL;
		}

		auto physical_address = physical::translate_linear(
			physical::m_stored_dtb,
			data.address );

		if ( !physical_address ) {
			return STATUS_UNSUCCESSFUL;
		}

		auto final_size = physical::find_min(
			PAGE_SIZE - ( physical_address & 0xFFF ),
			data.size );

		size_t bytes = 0;
		if ( !NT_SUCCESS( physical::write_physical(
			physical_address,
			reinterpret_cast< void* >( ( reinterpret_cast< ULONG64 >( data.buffer ) ) ),
			final_size,
			&bytes ) ) )
		{
			return STATUS_UNSUCCESSFUL;
		}

		return STATUS_SUCCESS;
	}


}