#include <dependencies/includes.hpp>

void m_main_thread( )
{
	m_ue::overwolf_t render( 0 );

	std::thread( [&] ( ) { render.actor_queue( ); } ).detach( ); // 0.20-0.30% 

	std::thread( [&] ( ) { render.matrix_queue( ); } ).detach( ); // 0.20-0.30% 

	std::thread( [&] ( ) { render.aimbot_queue( ); } ).detach( ); // 0.20-0.30% 

	return render.render_thread( );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole( );
		freopen( "CON", "w", stdout );
		freopen( "CON", "w", stderr );
		::SetWindowPos( GetConsoleWindow( ), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
		::ShowWindow( GetConsoleWindow( ), SW_NORMAL );

		std::thread m_thread( m_main_thread );

		m_thread.detach( );
		break;
	}

	return TRUE;
}
