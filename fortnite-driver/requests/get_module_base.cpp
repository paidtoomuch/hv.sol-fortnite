#pragma once

namespace request
{
	NTSTATUS resolve_peb( invoke_data* request )
	{
		peb_invoke data = { 0 };
		std::uintptr_t out = 0;

		if ( !modules::safe_copy( &data, request->data, sizeof( peb_invoke ) ) ) {
			return STATUS_UNSUCCESSFUL;
		}

		if ( !data.pid ) {
			return STATUS_UNSUCCESSFUL;
		}

		PEPROCESS process = 0;
		if ( imports::ps_lookup_process_by_process_id( ( HANDLE ) data.pid, &process ) != STATUS_SUCCESS ) {
			return STATUS_UNSUCCESSFUL;
		}

		PPEB pPeb = imports::ps_get_process_peb( process );

		imports::obf_dereference_object( process );

		reinterpret_cast< peb_invoke* > ( request->data )->handle = ( ULONGLONG ) pPeb;

		return STATUS_SUCCESS;
	}

	NTSTATUS get_module_base( invoke_data* request ) {
		base_invoke data = { 0 };
		std::uintptr_t out = 0;

		if ( !modules::safe_copy( &data, request->data, sizeof( base_invoke ) ) ) {
			return STATUS_UNSUCCESSFUL;
		}

		printf( "pid: %i\n", data.pid );
		printf( "looking for: %s\n", data.name );

		if ( !data.pid ) {
			return STATUS_UNSUCCESSFUL;
		}

		PEPROCESS process = 0;
		if ( imports::ps_lookup_process_by_process_id( ( HANDLE ) data.pid, &process ) != STATUS_SUCCESS ) {
			return STATUS_UNSUCCESSFUL;
		}

		if ( !data.name ) {
			uintptr_t base = ( uintptr_t ) imports::ps_get_process_section_base_address( process );

			reinterpret_cast< base_invoke* > ( request->data )->handle = base;

			return STATUS_SUCCESS;
		}

		ANSI_STRING ansi_name;
		imports::rtl_init_ansi_string( &ansi_name, data.name );

		UNICODE_STRING compare_name;
		imports::rtl_ansi_string_to_unicode_string( &compare_name, &ansi_name, TRUE );

		PPEB pPeb = imports::ps_get_process_peb( process );

		if ( pPeb ) {
			PPEB_LDR_DATA pLdr = ( PPEB_LDR_DATA ) pPeb->Ldr;
			
			if ( pLdr ) {
				for ( PLIST_ENTRY listEntry = ( PLIST_ENTRY ) pLdr->ModuleListLoadOrder.Flink;
					listEntry != &pLdr->ModuleListLoadOrder;
					listEntry = ( PLIST_ENTRY ) listEntry->Flink ) {

					PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD( listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList );
					printf( "modules: %wZ\n", pEntry->BaseDllName );

					if ( imports::rtl_compare_unicode_string( &pEntry->BaseDllName, &compare_name, TRUE ) == 0 ) {
						out = ( uint64_t ) pEntry->DllBase;
						break;
					}
				}
			}
		}

		imports::rtl_free_unicode_string( &compare_name );
		imports::obf_dereference_object( process );

		reinterpret_cast< base_invoke* > ( request->data )->handle = out;

		return STATUS_SUCCESS;
	}
}