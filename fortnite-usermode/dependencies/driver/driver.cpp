#include "driver.h"
#include <dependencies/xor.hpp>

bool m_ue::interface_t::start_service( ) {
	this->m_handle = CreateFileA( _( "\\\\.\\{9D34BA10-62B3-42A4-9383-89C3721AE019}" ), GENERIC_READ, 0, 0, 3, 0x00000080, 0 );

	if ( this->m_handle != INVALID_HANDLE_VALUE ) {
		return true;
	}

	printf( "failed handle.\n" );
	MessageBoxA( 0, "failed handle", 0, 0 );

	return false;
}


bool m_ue::interface_t::is_mapped( int proc ) {
	this->m_pid = proc;
	if ( !m_pid ) {
		return false;
	}

	return this->get_module_base( 0 );
}

bool m_ue::interface_t::resolve_dtb( ) {
	dtb_invoke data{ 0 };

	data.pid = this->m_pid;

	send_cmd( &data, invoke_resolve_dtb );

	return data.operation;
}

bool m_ue::interface_t::send_cmd( void* data, requests code ) {
	if ( !data || !code )
		return false;

	IO_STATUS_BLOCK block;

	invoke_data request{ 0 };

	request.unique = invoke_unique;
	request.data = data;
	request.code = code;

	DirectIO( this->m_handle, nullptr, nullptr, nullptr, &block, ( ULONG )nullptr, &request, sizeof( request ), &request, sizeof( request ) );

	return true;
}

std::uintptr_t m_ue::interface_t::get_module_base( const char* module_name ) {
	base_invoke data{ 0 };

	data.pid = this->m_pid;
	data.handle = 0;
	data.name = module_name;

	send_cmd( &data, invoke_base );

	return data.handle;
}

bool m_ue::interface_t::write( const std::uintptr_t address, void* buffer, const std::size_t size ) {
	write_invoke data{ 0 };

	data.pid = this->m_pid;
	data.address = address;
	data.buffer = buffer;
	data.size = size;

	return send_cmd( &data, invoke_write );
}

bool m_ue::interface_t::read( const std::uintptr_t address, void* buffer, const std::size_t size ) {
	read_invoke data{ 0 };

	data.pid = this->m_pid;
	data.address = address;
	data.buffer = buffer;
	data.size = size;

	return send_cmd( &data, invoke_read );
}
