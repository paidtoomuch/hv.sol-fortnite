
namespace m_ue
{
	class overwolf_t
	{
	public:
		std::vector<APlayerInformation> PlayerArray;

		bool InLobby = false;

		UWorld* GWorld;

		APawn* LocalPlayer;

		APlayerController* LocalPlayerController;

		int32_t LocalTeam;

		std::string LocalPlayerWeapon;

		AFortPlayerPawnAthena* TargetPlayer;

		FMinimalViewInfo m_camera_information;

		bool world_to_screen( FVector world_point, Vector2* to_screen_point );

		APlayerInformation* find_target( );

		float WorldGravityZ;

		FVector PredictLocation( FVector target, FVector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance )
		{
			float horizontalTime = distance / projectileSpeed;
			float verticalTime = distance / projectileSpeed;

			target.X += targetVelocity.X * horizontalTime;
			target.Y += targetVelocity.Y * horizontalTime;
			target.Z += targetVelocity.Z * verticalTime +
			abs( WorldGravityZ * projectileGravityScale ) * 0.5f * ( verticalTime * verticalTime );

			return target;
		}

	public:
		MSG m_msg = { nullptr };

		std::mutex m_mutex {};

		ID3D11Device* m_device{ nullptr };
		IDXGISwapChain* m_swapchain{ nullptr };
		ID3D11DeviceContext* m_device_context{ nullptr };
		ID3D11RenderTargetView* m_target_view{ nullptr };

	public:
		overwolf_t( HMODULE hModule );

		bool create_device( );
		bool create_target( );
		bool create_imgui( );
		bool hijack_overlay( );

		bool get_window( );
		HWND get_window_handle( DWORD pid );

		void end_scene( );
		void begin_scene( );
		bool get_screen_status( );

		void clean_context( );
		void release_objects( );

		void render_thread( );

		void menu_thread( );

		void debug_thread( );

	public:

		HINSTANCE m_instance{ };
		MARGINS m_margin{ -1 };

		RECT rect{ };
		DWORD m_pid{ };

		HWND m_ue{ };
		HWND m_overlay{ };

		ImFont* m_font{ };

		int m_width{ };
		int m_height{ };

		int m_width_center{ };
		int m_height_center{ };

		Vector2 m_center = Vector2( this->m_width_center, this->m_height_center );

	public:

		void draw_radar( int xAxis, int yAxis)
		{
			bool out = false;
			Vector2 siz;
			siz.x = xAxis;
			siz.y = yAxis;
			Vector2 pos;
			pos.x = m_cfg::radar::x_axis;
			pos.y = m_cfg::radar::y_axis;
			float RadarCenterX = pos.x + ( siz.x / 2 );
			float RadarCenterY = pos.y + ( siz.y / 2 );
			line( ImVec2(pos.x, RadarCenterY), ImVec2(pos.x + siz.x, RadarCenterY), ImColor{ 255, 255, 255, 255 }, 1.f );
			line( ImVec2(RadarCenterX, RadarCenterY), ImVec2(RadarCenterY, siz.y - 100), ImColor{ 255, 255, 255, 255 }, 1.f );
			line( ImVec2(RadarCenterX, RadarCenterY), ImVec2(RadarCenterX, pos.y + siz.y), ImColor{ 255, 255, 255, 255 }, 1.f );
			circle( Vector2(RadarCenterX, RadarCenterY), 2.f, ImColor{ 255, 255, 255, 255 }, 1.f );
		}

		void text( const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center );

		void filled_box( const ImVec2& position, const float width, const float height, const uint32_t color )
		{
			ImGui::GetBackgroundDrawList( )->AddRectFilled( position, ImVec2( position.x + width, position.y + height ), color, 0, 0 );
		}

		void normal_box( const ImVec2& position, const float width, const float height, const uint32_t color, float thickness )
		{
			// Top border
			line( position, ImVec2( position.x + width, position.y ), color, thickness );
			// Left border
			line( position, ImVec2( position.x, position.y + height ), color, thickness );
			// Right border
			line( ImVec2( position.x + width, position.y ), ImVec2( position.x + width, position.y + height ), color, thickness );
			// Bottom border
			line( ImVec2( position.x, position.y + height ), ImVec2( position.x + width, position.y + height ), color, thickness );
		}

		void outlined_box( const ImVec2& position, const float width, const float height, const uint32_t color, float thickness )
		{
			float lineW = ( width );
			float lineH = ( height );
			float lineT = 0.5;

			//outline
			line( ImVec2( position.x - lineT, position.y - lineT ), ImVec2( position.x + lineW, position.y - lineT ), ImColor( 0, 0, 0, 255 ), thickness ); //top left
			line( ImVec2( position.x - lineT, position.y - lineT ), ImVec2( position.x - lineT, position.y + lineH ), ImColor( 0, 0, 0, 255 ), thickness );
			line( ImVec2( position.x - lineT, position.y + height - lineH ), ImVec2( position.x - lineT, position.y + height + lineT ), ImColor( 0, 0, 0, 255 ), thickness );//bot left
			line( ImVec2( position.x - lineT, position.y + height + lineT ), ImVec2( position.x + lineW, position.y + height + lineT ), ImColor( 0, 0, 0, 255 ), thickness );
			line( ImVec2( position.x + width - lineW, position.y - lineT ), ImVec2( position.x + width + lineT, position.y - lineT ), ImColor( 0, 0, 0, 255 ), thickness ); // top right
			line( ImVec2( position.x + width + lineT, position.y - lineT ), ImVec2( position.x + width + lineT, position.y + lineH ), ImColor( 0, 0, 0, 255 ), thickness );
			line( ImVec2( position.x + width + lineT, position.y + height - lineH ), ImVec2( position.x + width + lineT, position.y + height + lineT ), ImColor( 0, 0, 0, 255 ), thickness ); // bot right
			line( ImVec2( position.x + width - lineW, position.y + height + lineT ), ImVec2( position.x + width + lineT, position.y + height + lineT ), ImColor( 0, 0, 0, 255 ), thickness );

			//inline
			line( ImVec2( position.x, position.y ), ImVec2( position.x, position.y + lineH ), color, thickness );//top left
			line( ImVec2( position.x, position.y ), ImVec2( position.x + lineW, position.y ), color, thickness );
			line( ImVec2( position.x + width - lineW, position.y ), ImVec2( position.x + width, position.y ), color, thickness ); //top right
			line( ImVec2( position.x + width, position.y ), ImVec2( position.x + width, position.y + lineH ), color, thickness );
			line( ImVec2( position.x, position.y + height - lineH ), ImVec2( position.x, position.y + height ), color, thickness ); //bot left
			line( ImVec2( position.x, position.y + height ), ImVec2( position.x + lineW, position.y + height ), color, thickness );
			line( ImVec2( position.x + width - lineW, position.y + height ), ImVec2( position.x + width, position.y + height ), color, thickness );//bot right
			line( ImVec2( position.x + width, position.y + height - lineH ), ImVec2( position.x + width, position.y + height ), color, thickness );

		}

		void cross( const Vector2& pos, float size, uint32_t color )
		{
			this->text( _( "+" ), ImVec2( pos.x, pos.y ), size, color, true );
		}

		void circle( const Vector2& pos, int radius, uint32_t color, int segments )
		{
			ImGui::GetBackgroundDrawList( )->AddCircle( ImVec2( pos.x, pos.y ), radius, color, segments );
		}

		void circle_filled( const Vector2& pos, int radius, uint32_t color, int segments )
		{
			ImGui::GetBackgroundDrawList( )->AddCircleFilled( ImVec2( pos.x, pos.y ), radius, color, segments );
		}
		void rec_filled( float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags )
		{
			ImGui::GetBackgroundDrawList( )->AddRectFilled( ImVec2( x0, y0 ), ImVec2( x1, y1 ), color, rounding, rounding_corners_flags );

		}
		void progress_bar( float x, float y, float w, float h, int value, int v_max )
		{
			float progress_height = ( h / static_cast< float >( v_max ) ) * static_cast< float >( value );
			float bar_y = y + h - progress_height;

			ImColor bar_color = ImColor{
				min( 510 * ( v_max - value ) / v_max, 255 ),
				min( 510 * value / v_max, 255 ),
				25,
				255
			};
			rec_filled( x, bar_y, x + w, y + h, bar_color, 0.0f, 0 );
		}

		void line( const ImVec2& from, const ImVec2& to, uint32_t color, float thickness );

	public:
		bool actor_queue( );
		bool matrix_queue( );
		bool aimbot_queue( );

		bool skeletons( USkeletalMeshComponent* Mesh, UObject* BoneArray )
		{
			std::list<int> upper_part = { bones::HumanNeck,bones::HumanHead };
			std::list<int> right_arm = { bones::HumanNeck,bones::HumanChest,bones::HumanRUpperarm,bones::HumanRForearm1,bones::HumanRForearm23,bones::HumanRPalm };
			std::list<int> left_arm = { bones::HumanNeck,bones::HumanChest,bones::HumanLUpperarm,bones::HumanLForearm1,bones::HumanLForearm23,bones::HumanLPalm };
			std::list<int> spine = { bones::HumanChest,bones::HumanSpine1,bones::HumanSpine2,bones::HumanSpine3 };

			std::list<int> lower_right = { bones::HumanPelvis,bones::HumanRThigh1,bones::HumanRThigh2,bones::HumanRThigh3,bones::HumanRCalf,bones::HumanRFoot };
			std::list<int> lower_left = { bones::HumanPelvis,bones::HumanLThigh1,bones::HumanLThigh2,bones::HumanLThigh3,bones::HumanLCalf,bones::HumanLFoot };
			std::list<std::list<int>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };

			FVector previous( 0, 0, 0 );
			FVector current;
			auto p1 = Vector2( 0, 0 );
			auto c1 = Vector2( 0, 0 );
			auto neckpos = Mesh->GetBoneLocation( BoneArray, bones::HumanNeck );
			auto pelvispos = Mesh->GetBoneLocation( BoneArray, bones::HumanPelvis );

			for ( auto a : skeleton ) {
				previous = FVector( 0, 0, 0 );
				for ( int bone : a ) {
					ImColor Color = { 0,255,255, 255 };

					current = current = bone == bones::HumanNeck ? neckpos : ( bone == bones::HumanPelvis ? pelvispos : Mesh->GetBoneLocation( BoneArray, bone ) );
					if ( previous.X == 0.f ) {
						previous = current;
						continue;
					}

					if ( this->world_to_screen( current, &c1 ) && this->world_to_screen( previous, &p1 ) ) {
						this->line( ImVec2( p1.x, p1.y ), ImVec2( c1.x, c1.y ), Color, 1 );
						previous = current;
					}
				}
			}
		}

		bool get_player_dimensions( USkeletalMeshComponent* mesh, UObject* bone_array, bounds::bounds_t* out_bounds )
		{
			const auto get_bounds = [&] ( bounds::bounds_t& out, float expand = 0.f ) -> bool
			{
				bounds::bounds_t bounds = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

				for ( auto& bone : bounds::BonesArray ) {
					FVector bone_pos = mesh->GetBoneLocation(bone_array, bone.index );

					bone.screen = Vector2{  };

					if ( bone.index == bones::HumanHead )
						bone_pos.Z += 15;

					if ( bone.index == bones::HumanLCalf ||
						bone.index == bones::HumanRCalf) {
						bone_pos.Z -= 8.5;
					}

					if ( bone.index == bones::HumanRPalm ||
						bone.index == bones::HumanRForearm23 ) {
						bone_pos.X -= 10.5;
					}

					if ( bone.index == bones::HumanLPalm ||
						bone.index == bones::HumanLForearm23 ) {
						bone_pos.X += 11;
					}

					if ( this->world_to_screen( bone_pos, &bone.screen ) ) {
						if ( bone.screen.x < bounds.left )
							bounds.left = bone.screen.x;
						else if ( bone.screen.x > bounds.right )
							bounds.right = bone.screen.x;
						if ( bone.screen.y < bounds.top )
							bounds.top = bone.screen.y;
						else if ( bone.screen.y > bounds.bottom )
							bounds.bottom = bone.screen.y;

						bone.on_screen = true;
					}

				}

				if ( bounds.left == FLT_MAX )
					return false;
				if ( bounds.right == FLT_MIN )
					return false;
				if ( bounds.top == FLT_MAX )
					return false;
				if ( bounds.bottom == FLT_MIN )
					return false;

				bounds.left -= expand;
				bounds.right += expand;
				bounds.top -= expand;
				bounds.bottom += expand;

				out = bounds;

				return true;
			};

			bounds::bounds_t bounds;

			if ( get_bounds( bounds, 4.f ) ) {
				*out_bounds = bounds;
				return true;
			}

			return false;

		}
	};
}
