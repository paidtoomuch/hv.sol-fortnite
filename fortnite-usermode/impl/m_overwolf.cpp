#pragma once
#include <dependencies/includes.hpp>

using namespace m_ue;

// constructor
m_ue::overwolf_t::overwolf_t( HMODULE hModule )
{
	auto m_fn_pid = utl::get_process_id( _( L"FortniteClient-Win64-Shipping.exe" ) );
	if ( !m_fn_pid ) {
		std::printf( _( "failed to get game pid.\n" ) );
		return;
	}

	this->m_pid = m_fn_pid;
	this->m_ue = this->get_window_handle( m_fn_pid );

	auto get_window = this->get_window( );
	if ( !get_window ) {
		std::printf( _( "failed to get window info.\n" ) );
		return;
	}

	auto hijack_overlay = this->hijack_overlay( );
	if ( !hijack_overlay ) {
		std::printf( _( "failed to hijack overlay.\n" ) );
		return;
	}

	auto create_swapchain = this->create_device( );
	if ( !create_swapchain ) {
		std::printf( _( "failed to create swapchain.\n" ) );
		return;
	}

	auto create_render_view = this->create_target( );
	if ( !create_render_view ) {
		std::printf( _( "failed to create target view.\n" ) );
		return;
	}

	auto create_imgui = this->create_imgui( );
	if ( !create_imgui ) {
		std::printf( _( "failed to create imgui.\n" ) );
		return;
	}
}

auto m_ue::overwolf_t::get_window( ) -> bool
{
	auto result = GetWindowRect( this->m_ue, &this->rect );
	if ( !result ) {
		return false;
	}

	this->m_width = rect.right - rect.left;
	this->m_height = rect.bottom - rect.top;

	this->m_width_center = m_width / 2;
	this->m_height_center = m_height / 2;

	return true;
}

auto m_ue::overwolf_t::get_window_handle( DWORD pid ) -> HWND
{
	std::pair<HWND, DWORD> params = { 0, pid };

	BOOL bResult = EnumWindows( [] ( HWND hwnd, LPARAM lParam ) -> BOOL
		{
			auto pParams = ( std::pair<HWND, DWORD>* )( lParam );

			DWORD processId;
			if ( GetWindowThreadProcessId( hwnd, &processId ) && processId == pParams->second ) {
				// Stop enumerating
				SetLastError( -1 );
				pParams->first = hwnd;
				return FALSE;
			}

			// Continue enumerating
			return TRUE;
		},
		( LPARAM ) &params );

	if ( !bResult && GetLastError( ) == -1 && params.first ) {
		return params.first;
	}

	return 0;
}

auto m_ue::overwolf_t::create_target( ) -> bool
{
	ID3D11Texture2D* render_buffer{ nullptr };
	auto result = this->m_swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &render_buffer ) );
	if ( FAILED( result ) ) {
		return false;
	}

	result = this->m_device->CreateRenderTargetView( render_buffer, nullptr, &this->m_target_view );
	if ( FAILED( result ) ) {
		return false;
	}

	render_buffer->Release( );
	return true;
}

auto m_ue::overwolf_t::create_device( ) -> bool
{
	// refresh rate
	DXGI_RATIONAL refresh_rate{};
	ZeroMemory( &refresh_rate, sizeof( DXGI_RATIONAL ) );
	refresh_rate.Numerator = 0;
	refresh_rate.Denominator = 1;

	// buffer
	DXGI_MODE_DESC buffer_desc{};
	ZeroMemory( &buffer_desc, sizeof( DXGI_MODE_DESC ) );
	buffer_desc.Width = this->m_width;
	buffer_desc.Height = this->m_height;
	buffer_desc.RefreshRate = refresh_rate;
	buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// sample
	DXGI_SAMPLE_DESC sample_desc{};
	ZeroMemory( &sample_desc, sizeof( DXGI_SAMPLE_DESC ) );
	sample_desc.Count = 1;
	sample_desc.Quality = 0;

	// Swapchain
	DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	ZeroMemory( &swapchain_desc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	swapchain_desc.BufferDesc = buffer_desc;
	swapchain_desc.SampleDesc = sample_desc;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2;
	swapchain_desc.OutputWindow = this->m_overlay;
	swapchain_desc.Windowed = TRUE;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto ret = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		0,
		0,
		D3D11_SDK_VERSION,
		&swapchain_desc,
		&this->m_swapchain,
		&this->m_device,
		0,
		&this->m_device_context );

	if ( FAILED( ret ) ) {
		return false;
	}
}

auto m_ue::overwolf_t::create_imgui( ) -> bool
{
	ImGui::CreateContext( );

	auto imgui_win32 = ImGui_ImplWin32_Init( this->m_overlay );
	if ( !imgui_win32 ) {
		std::printf( _( "failed to load imgui Win32.\n" ) );
		return false;
	}

	auto imgui_dx11 = ImGui_ImplDX11_Init( this->m_device, this->m_device_context );
	if ( !imgui_dx11 ) {
		std::printf( _( "failed to load imgui.\n" ) );
		return false;
	}

	ImGuiIO& io = ImGui::GetIO( );
	ImFontConfig fontCfg = ImFontConfig( );
	io.DeltaTime = 1.0f / 60.0f;

	fontCfg.OversampleH = fontCfg.OversampleV = 1;
	fontCfg.PixelSnapH = true;

	this->m_font = ImGui::GetIO( ).Fonts->AddFontFromFileTTF( _( "C:\\Windows\\Fonts\\Arial.ttf" ), 14.0f );

	return true;
}

auto m_ue::overwolf_t::hijack_overlay( ) -> bool
{
	this->m_overlay = FindWindowA( _( "CiceroUIWndFrame" ), _( "CiceroUIWndFrame" ) );
	if ( !this->m_overlay ) {
		std::printf( _( "failed to find overlay.\n" ) );
		return false;
	}

	// you can check if these all work or not but if they fail you got a bigger problem LOL
	ShowWindow( this->m_overlay, SW_SHOW );

	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea( this->m_overlay, &margins );
	SetWindowLongA( this->m_overlay, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT );
	SetWindowPos( this->m_overlay, 0, 0, 0, this->m_width, this->m_height, SWP_NOREDRAW );
	UpdateWindow( this->m_overlay );

	auto c1( xParseByteArray( "48 89 5C 24 08 57 48 83 EC 30 49 8B C0 48 8B FA 48 8B D0 4C 8D 05 CC CC CC CC 48 8B CF E8 CC CC CC CC 8B D8 85 C0 78 09" ) );
	auto c2( xParseByteArray( "48 89 5C 24 08 57 48 83 EC 30 49 8B C0 48 8B FA 48 8B D0 4C 8D 05 CC CC CC CC 48 8B CF B8 00 00 00 00 8B D8 85 C0 78 09" ) );

	xPatchProcess( _( L"dwm.exe"), c1, c2, NULL, 0 );

	return true;
}

auto m_ue::overwolf_t::end_scene( ) -> void
{
	const float color[]{ 0, 0, 0, 0 };

	ImGui::Render( );
	ImGui::EndFrame( );

	this->m_device_context->OMSetRenderTargets( 1, &this->m_target_view, nullptr );
	this->m_device_context->ClearRenderTargetView( this->m_target_view, color );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	// 1, 0 for vsync.
	this->m_swapchain->Present( 1, 0 );
}

auto m_ue::overwolf_t::release_objects( ) -> void
{
	if ( this->m_target_view ) {

		this->m_target_view->Release( );
		this->m_target_view = nullptr;
	}

	if ( this->m_device_context ) {

		this->m_device_context->Release( );
		this->m_device_context = nullptr;
	}

	if ( this->m_device ) {

		this->m_device->Release( );
		this->m_device = nullptr;
	}

	if ( this->m_swapchain ) {

		this->m_swapchain->Release( );
		this->m_swapchain = nullptr;
	}
}

auto m_ue::overwolf_t::clean_context( ) -> void
{
	ImGui_ImplDX11_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );
	DestroyWindow( this->m_overlay );
}

auto m_ue::overwolf_t::begin_scene( ) -> void
{
	ImGui_ImplDX11_NewFrame( );
	POINT p;
	GetCursorPos( &p );
	ImGuiIO& io = ImGui::GetIO( );
	io.MousePos = ImVec2( p.x, p.y );
	io.MouseDown[0] = ( GetKeyState( VK_LBUTTON ) & 0x8000 ) != 0;
	io.MouseDown[1] = ( GetKeyState( VK_RBUTTON ) & 0x8000 ) != 0;
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );
}

auto m_ue::overwolf_t::get_screen_status( ) -> bool
{
	if ( this->m_ue == GetForegroundWindow( ) ) {
		return true;
	}

	if ( this->m_ue == GetActiveWindow( ) ) {
		return true;
	}

	if ( GetActiveWindow( ) == GetForegroundWindow( ) ) {
		return true;
	}

	return false;
}

void m_ue::overwolf_t::menu_thread( )
{

	ImGui::StyleColorsDark( );

	constexpr auto ColorFromBytes = [] ( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 )
	{
		return ImVec4( ( float ) r / 255.0f, ( float ) g / 255.0f, ( float ) b / 255.0f, ( float ) a / 255.0f );
	};
	ImGuiStyle* style = &ImGui::GetStyle( );
	ImVec4* colors = style->Colors;

	const ImVec4 bgColor = ColorFromBytes( 37, 37, 38 );
	const ImVec4 lightBgColor = ColorFromBytes( 82, 82, 85 );
	const ImVec4 veryLightBgColor = ColorFromBytes( 90, 90, 95 );
	const ImVec4 veryLightBgColor_1 = ColorFromBytes( 130, 130, 135 );

	const ImVec4 panelColor = ColorFromBytes( 51, 51, 51 );
	const ImVec4 panelHoverColor = ColorFromBytes( 255, 255, 255 );
	const ImVec4 panelActiveColor = ColorFromBytes( 255, 255, 255 );
	const ImVec4 SliderColor = ColorFromBytes( 255, 255, 255, 125 );

	const ImVec4 textColor = ColorFromBytes( 255, 255, 255 );
	const ImVec4 textDisabledColor = ColorFromBytes( 151, 151, 151 );
	const ImVec4 borderColor = ColorFromBytes( 78, 78, 78 );

	colors[ImGuiCol_Text] = textColor;
	colors[ImGuiCol_TextDisabled] = textDisabledColor;
	colors[ImGuiCol_TextSelectedBg] = ColorFromBytes( 130, 130, 135, 100 );
	colors[ImGuiCol_WindowBg] = bgColor;
	colors[ImGuiCol_PopupBg] = bgColor;
	colors[ImGuiCol_Border] = borderColor;
	colors[ImGuiCol_BorderShadow] = borderColor;
	colors[ImGuiCol_FrameBg] = panelColor;
	colors[ImGuiCol_FrameBgHovered] = veryLightBgColor;
	colors[ImGuiCol_FrameBgActive] = veryLightBgColor_1;
	colors[ImGuiCol_TitleBg] = bgColor;
	colors[ImGuiCol_TitleBgActive] = bgColor;
	colors[ImGuiCol_TitleBgCollapsed] = bgColor;
	colors[ImGuiCol_MenuBarBg] = panelColor;
	colors[ImGuiCol_ScrollbarBg] = panelColor;
	colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
	colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
	colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
	colors[ImGuiCol_CheckMark] = panelActiveColor;
	colors[ImGuiCol_SliderGrab] = SliderColor;
	colors[ImGuiCol_SliderGrabActive] = SliderColor;
	colors[ImGuiCol_Button] = panelColor;
	colors[ImGuiCol_ButtonHovered] = veryLightBgColor_1;
	colors[ImGuiCol_ButtonActive] = veryLightBgColor_1;
	colors[ImGuiCol_Header] = panelColor;
	colors[ImGuiCol_HeaderHovered] = veryLightBgColor;
	colors[ImGuiCol_HeaderActive] = veryLightBgColor_1;
	colors[ImGuiCol_Separator] = borderColor;
	colors[ImGuiCol_SeparatorHovered] = borderColor;
	colors[ImGuiCol_SeparatorActive] = borderColor;
	colors[ImGuiCol_ResizeGrip] = bgColor;
	colors[ImGuiCol_ResizeGripHovered] = panelColor;
	colors[ImGuiCol_ResizeGripActive] = lightBgColor;
	colors[ImGuiCol_PlotLines] = panelActiveColor;
	colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
	colors[ImGuiCol_PlotHistogram] = panelActiveColor;
	colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
	colors[ImGuiCol_DragDropTarget] = bgColor;
	colors[ImGuiCol_NavHighlight] = bgColor;
	colors[ImGuiCol_Tab] = bgColor;
	colors[ImGuiCol_TabActive] = panelActiveColor;
	colors[ImGuiCol_TabUnfocused] = bgColor;
	colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
	colors[ImGuiCol_TabHovered] = panelHoverColor;

	style->SelectableTextAlign = ImVec2( 0.5f, 0.5f );
	style->WindowPadding = ImVec2( 8.f, 5.f );
	style->WindowRounding = 0.0f;
	style->FrameRounding = 2.0f;
	style->GrabRounding = 2.0f;
	style->ScrollbarRounding = 2.0f;
	style->ScrollbarSize = 15.f;
	style->WindowTitleAlign = ImVec2( 0.5f, 0.5f );

	//if ( ImGui::Begin( "radar", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize ) ) {
	//	draw_radar( ImGui::GetWindowSize( ).x, ImGui::GetWindowSize( ).y );
	//	ImGui::End( );

	//}

	ImGui::Begin( _( "Shitcode Community: FN External - contact elkirat2014" ), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize );
	ImGui::Separator( );

	static int m_tab = 0;

	if ( ImGui::Button( _( "visuals" ), ImVec2( 120, 20 ) ) ) {
		m_tab = 0;
	}

	ImGui::SameLine( );
	if ( ImGui::Button( _( "aimbot" ), ImVec2( 120, 20 ) ) ) {
		m_tab = 1;
	}

	ImGui::SameLine( );
	if ( ImGui::Button( _( "radar" ), ImVec2( 120, 20 ) ) ) {
		m_tab = 2;
	}

	ImGui::Spacing( );

	if ( m_tab == 0 ) {
		ImGui::Checkbox( _( "name" ), &m_cfg::player::m_name );
		ImGui::Checkbox( _( "distance" ), &m_cfg::player::m_distance );
		ImGui::Checkbox( _( "bounding box" ), &m_cfg::player::m_box );
		ImGui::Checkbox( _( "view angle" ), &m_cfg::player::m_view_angle );
		if ( ImGui::TreeNode( _( "configuration" ) ) ) {
			ImGui::Text( _( "length" ) );
			ImGui::SliderFloat( _( "mp_view_angle_length" ), &m_cfg::player::m_view_angle_distance, 1, 300, _( "%.3g" ) );

			ImGui::TreePop( );
		}
		ImGui::Checkbox( _( "skeleton" ), &m_cfg::player::m_skeleton );
		ImGui::Checkbox( _( "current weapon" ), &m_cfg::player::m_current_gun );

		ImGui::Separator( );

		ImGui::Text( _( "render distance" ) );
		ImGui::SliderFloat( _( "mp_render_distance" ), &m_cfg::m_esp_render_distance, 1, 450, _( "%.3g" ) );
		ImGui::Checkbox( _( "render teammates" ), &m_cfg::player::m_team );

	}

	if ( m_tab == 1 ) {
		ImGui::Checkbox( _( "memory" ), &m_cfg::aim::m_memory );
		if ( ImGui::TreeNode( _( "configuration" ) ) ) {
			ImGui::Text( _( "radius" ) );
			ImGui::SliderFloat( _( "mp_field_of_view" ), &m_cfg::aim::m_fov, 1, 120, _( "%.3g" ) );

			ImGui::Text( _( "target bone" ) );
			ImGui::Combo( _( "mp_target_bone" ), &m_cfg::aim::m_target_bone, m_cfg::aim::m_bone_names, IM_ARRAYSIZE( m_cfg::aim::m_bone_names ) );

			ImGui::Text( _( "smoothness" ) );
			ImGui::SliderFloat( _( "mp_smoothness" ), &m_cfg::aim::m_smoothness, 1, 30, _( "%.2g" ) );

			ImGui::Text( _( "aimbot distance" ) );
			ImGui::SliderFloat( _( "mp_aimbot_distance" ), &m_cfg::aim::m_aimbot_distance, 1, 450, _( "%.3g" ) );

			ImGui::Text( _( "sniper & pistol prediction, use with caution.\nheadshot only and deadly accurate." ) );
			ImGui::Checkbox( _( "prediction" ), &m_cfg::aim::m_prediction );
			ImGui::TreePop( );
		}

		if ( m_cfg::aim::m_memory ) {
			ImGui::Checkbox( _( "fov circle" ), &m_cfg::aim::m_draw_fov );

		}
	}

	if ( m_tab == 2 ) {
		ImGui::Checkbox( _( "streamproof" ), &m_cfg::m_streamproof );

		if ( ImGui::TreeNode( _( "configuration" ) ) ) {
			ImGui::Text( _( "render teammates" ) );
			ImGui::Checkbox( _( "render teammates" ), &m_cfg::radar::m_draw_team );

			ImGui::TreePop( );
		}
	}


}

void m_ue::overwolf_t::debug_thread( )
{
	std::string debug_list;

	this->text( debug_list, ImVec2( 21, 100 ), 14.f, ImColor( 255, 255, 255, 245 ), false );
}

void m_ue::overwolf_t::render_thread( )
{
	while ( this->m_msg.message != WM_QUIT ) {
		if ( PeekMessage( &this->m_msg, this->m_overlay, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &this->m_msg );
			DispatchMessage( &this->m_msg );
		}

		if ( GetAsyncKeyState( VK_INSERT ) & 1 )
			m_cfg::m_show_menu = !m_cfg::m_show_menu;

		if ( GetAsyncKeyState( VK_DELETE ) & 1 )
			exit( 0 );

		if ( !utl::get_process_id( _( L"FortniteClient-Win64-Shipping.exe" ) ) )
			exit( 0 );

		this->begin_scene( );
		{
			if ( this->get_screen_status( ) ) {
				if ( m_cfg::m_show_menu ) {
					this->menu_thread( );
				}

				//if (m_cfg::m_debug)
				//{
				//	this->debug_thread();
				//}

				if ( m_cfg::m_streamproof ) {
					XGUARD_WIN( SetWindowDisplayAffinity( this->m_overlay, WDA_EXCLUDEFROMCAPTURE ) );
					m_cfg::proof = 1;
				}
				else if ( m_cfg::proof ) {
					XGUARD_WIN( SetWindowDisplayAffinity( this->m_overlay, WDA_NONE ) );
				}

				this->text( _( "Fortnite v5.11" ), ImVec2( 21, 17 ), 15.0f, ImColor( 255, 255, 255, 245 ), false );

				if ( m_cfg::m_crosshair ) {
					constexpr long crosshair_size = 10.0f;

					ImVec2 center = ImVec2( this->m_width_center, this->m_height_center );

					this->line( ImVec2( ( center.x ), ( center.y ) - crosshair_size ), ImVec2( ( center.x ), ( center.y ) + crosshair_size ), ImGui::GetColorU32( { 255, 0, 0, 255 } ), 1.5f );
					this->line( ImVec2( ( center.x ) - crosshair_size, ( center.y ) ), ImVec2( ( center.x ) + crosshair_size, ( center.y ) ), ImGui::GetColorU32( { 255, 0, 0, 255 } ), 1.5f );
				}

				if ( m_cfg::aim::m_draw_fov ) {
					float aim_radius = ( m_cfg::aim::m_fov * this->m_width_center / this->m_camera_information.FOV ) / 2;
					this->circle( Vector2( this->m_width_center, this->m_height_center ), aim_radius, ImColor( 255, 255, 255 ), 250 );
				}

				if ( this->GWorld ) {
					auto ClosestDistance = DBL_MAX;
					auto ClosestCrosshairDistance = DBL_MAX;

					AFortPlayerPawnAthena* ClosestPlayerByDistance = NULL;
					AFortPlayerPawnAthena* ClosetPlayerByCrosshair = NULL;

					for ( auto PawnInfo : this->PlayerArray ) {
						auto Pawn = PawnInfo.FortnitePawn;
						auto Mesh = PawnInfo.Mesh;
						auto BoneArray = PawnInfo.BoneArray;
						auto BaseBonePosition = PawnInfo.BaseBonePosition;
						auto CurrentWeapon = PawnInfo.WeaponName;

						auto Distance = m_camera_information.Location.Distance( BaseBonePosition ) * 0.01;

						auto PlayerState = PawnInfo.PlayerState;

						auto LocalPlayerPawn = reinterpret_cast< AFortPlayerPawnAthena* >( this->LocalPlayer );

						//if ( m_cfg::radar::m_radar ) {

						//}

						Vector2 ScreenBaseBone;

						if ( Distance < ClosestDistance ) {
							ClosestPlayerByDistance = Pawn;
							ClosestDistance = Distance;
						}

						if ( Distance > m_cfg::m_esp_render_distance )
							continue;

						if ( this->world_to_screen( BaseBonePosition, &ScreenBaseBone ) ) {

							float crosshair_distance = Vector2( this->m_width_center, this->m_height_center ).Distance( ScreenBaseBone );

							if ( crosshair_distance < ClosestCrosshairDistance ) {
								ClosetPlayerByCrosshair = Pawn;
								ClosestCrosshairDistance = crosshair_distance;
							}
						}

						if ( Pawn->IsInScreen( ScreenBaseBone, this->m_width, this->m_height ) ) {
							this->TargetPlayer = ClosetPlayerByCrosshair;
							auto target_screen = Vector2( );

							if ( ( this->world_to_screen( BaseBonePosition, &target_screen ) && target_screen && customs::in_circle( Vector2( this->m_width_center, this->m_height_center ).x, Vector2( this->m_width_center, this->m_height_center ).y, 75.0f, target_screen.x, target_screen.y ) ) || Distance < 95.0f ) {

								if ( m_cfg::player::m_box || m_cfg::player::m_distance || m_cfg::player::m_current_gun || m_cfg::player::m_reloading_status || m_cfg::player::m_name || m_cfg::player::m_view_angle ) {
									bounds::bounds_t o { };
									if ( this->get_player_dimensions( Mesh, BoneArray, &o ) ) {

										if ( m_cfg::player::m_box ) {
											ImColor corner_box_color( 255, 165, 0, 255 );

											this->outlined_box( ImVec2( o.left, o.bottom ), o.right - o.left, o.top - o.bottom, corner_box_color, 1.0f );
										}

										if ( m_cfg::player::m_view_angle ) {
											FVector RelativeRotation = Pawn->RootComponent( )->RelativeRotation( );
											FVector RelativeLocation = InLobby ? this->m_camera_information.Rotation : Mesh->GetBoneLocation( BoneArray, bones::HumanHead );

											FVector test2, headpos;

											headpos = RelativeLocation;

											customs::angle_rotation( RelativeRotation, &test2 );
											test2.X *= m_cfg::player::m_view_angle_distance;
											test2.Y *= m_cfg::player::m_view_angle_distance;
											test2.Z *= m_cfg::player::m_view_angle_distance;

											FVector end = headpos + test2;
											Vector2 test1, test3;

											if ( this->world_to_screen( headpos, &test1 ) ) {
												if ( this->world_to_screen( end, &test3 ) ) {
													this->line( { test1.x, test1.y }, { test3.x, test3.y }, ImColor( 255, 0, 0, 255 ), 1 );
												}
											}
										}

										if ( m_cfg::player::m_name ) {
											ImVec2 textPosition( o.left + ( o.right - o.left ) / 2.0f, o.top - 20.0f ); // Adjust the vertical offset as needed

											std::string playerName = PlayerState->username( InLobby ).c_str( );

											//if (/* this->InLobby ||*/ playerName.empty( ) ) {
											//	playerName = _( "discord.gg/hvs" );
											//}

											if ( PlayerState->is_bot( ) ) {
												playerName = _( "BOT/AI" );
											}

											this->text( playerName, textPosition, 16.0f, ImColor( 255, 165, 0, 255 ), true );
										}

										if ( m_cfg::player::m_distance ) {

											ImVec2 textPosition( o.left + ( o.right - o.left ) / 2.0f, o.bottom + 7.0f ); // Adjust the vertical offset as needed

											std::string textDistance = std::to_string( ( int ) ( Distance ) ) + _( "m" );

											this->text( textDistance, textPosition, 16.0f, ImColor( 255, 165, 0, 255 ), true );
										}

										if ( m_cfg::player::m_current_gun ) {

											ImVec2 textPosition( o.left + ( o.right - o.left ) / 2.0f, o.bottom + 21.0f ); // Adjust the vertical offset as needed

											this->text( PawnInfo.WeaponInfo.weapon_name, textPosition, 16.0f, ImColor( 0, 255, 255, 255 ), true );
										}
									}
								}

								if ( m_cfg::player::m_skeleton ) {
									this->skeletons( Mesh, BoneArray );
								}
							}
							else {
								Vector2 out;
								if ( this->world_to_screen( BaseBonePosition, &out ) ) {
									ImColor corner_box_color( 255, 165, 0, 255 );
									this->cross( out, 35.0f, corner_box_color );
								}
							}

						}

						if (/* !ClosestPlayerByDistance &&*/ !ClosetPlayerByCrosshair ) {
							TargetPlayer = 0;
						}
					}
				}
			}
		}

		this->end_scene( );

		//std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

	ImGui_ImplDX11_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );

	DestroyWindow( this->m_overlay );
}