#pragma once 
#include <dependencies/includes.hpp>

using namespace m_ue;

using namespace m_cfg;

auto m_ue::overwolf_t::matrix_queue( ) -> bool
{
	for ( ;; ) {
		if ( !this->GWorld ) {
			continue;
		}

		char v1; // r8
		this->m_camera_information = device_t.read<FMinimalViewInfo>( device_t.m_base + 0xED57310 );
		BYTE* v2 = ( BYTE* ) &this->m_camera_information;
		int i; // edx
		__int64 result; // rax

		v1 = 0x40;
		for ( i = 0; i < 0x40; ++i ) {
			*v2 ^= v1;
			result = ( unsigned int ) ( i + 0x17 );
			v1 += i + 0x17;
			v2++;
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
}

auto m_ue::overwolf_t::actor_queue( ) -> bool
{
	if ( !device_t.start_service( ) )
		return FALSE;

	device_t.m_pid = utl::get_process_id( _( L"FortniteClient-Win64-Shipping.exe" ) );

	if ( device_t.is_mapped( device_t.m_pid ) ) {

		device_t.m_base = device_t.get_module_base( 0 );

		if ( !device_t.m_base ) {
			m_log( "invalid base address: 0x%llx.\n", device_t.m_base );
			return FALSE;
		}

		m_log( "m_base: 0x%llx.\n", device_t.m_base );

		if ( !device_t.resolve_dtb( ) ) {
			m_log( "failed to get dtb.\n" );
			return FALSE;
		}

		for ( ;; ) {
			std::vector<APlayerInformation> m_entities;

			this->GWorld = UWorld::Get( );
			if ( !this->GWorld ) {
				m_log( "invalid uworld\n" );
				continue;
			}

			const auto OwningGameInstance = GWorld->OwningGameInstance( );
			if ( !OwningGameInstance ) {
				continue;
			}

			const auto GameState = GWorld->GameState( );
			if ( !GameState ) {
				continue;
			}

			const auto ServerTime = GameState->ServerWorldTimeSecondsDelta( );
			if ( ServerTime == 0.0f ) {
				InLobby = true;
			}
			else {
				InLobby = false;
			}

			//auto LocalPlayerPtr = OwningGameInstance->GetLocalPlayer( );
			//if ( !LocalPlayerPtr ) {
			//	continue;
			//}

			//this->LocalPlayer = LocalPlayerPtr->PlayerController( )->AcknowledgedPawn( );

			//auto LocalController = LocalPlayerPtr->PlayerController( );

			//printf( "local controller: %llx\n", LocalController );

			//if ( LocalPlayerController ) {
			//}

			const auto PersistentLevel = GWorld->PersistentLevel( );
			if ( !PersistentLevel ) {
				continue;
			}

			const auto WorldSettings = PersistentLevel->WorldSettingsClass( );
			if ( !WorldSettings ) {
				continue;
			}

			this->WorldGravityZ = WorldSettings->WorldGravityZ( );

			auto PlayerArray = GWorld->GameState( )->PlayerArray( );

			for ( int i = 0; i < PlayerArray.Count; i++ ) {
				if ( !PlayerArray.MaxCount ) {
					continue;
				}

				APlayerInformation PlayerInfo;

				auto CurrentState = reinterpret_cast< APlayerState* >( PlayerArray.Get( i ) );

				auto CurrentPawn = reinterpret_cast< AFortPlayerPawnAthena* >( CurrentState->PawnPrivate( ) );

				auto CurrentController = CurrentPawn->PlayerController( );

				if ( CurrentPawn->PlayerController( )->IsLocalController( ) ) {
					this->LocalPlayer = CurrentPawn;
					this->LocalPlayerController = CurrentController;
					this->LocalTeam = CurrentState->TeamIndex( );
				}

				if ( device_t.is_valid( ( uint64_t ) CurrentPawn ) ) {
					if ( !InLobby && m_cfg::player::m_current_gun ) {
						WeaponInformation held_weapon{};

						uint64_t player_weapon = device_t.read<uint64_t>( reinterpret_cast< uint64_t >( CurrentPawn ) + 0x948 );

						if ( device_t.is_valid(player_weapon) ) {
							held_weapon.ammo_count = device_t.read<int32_t>( player_weapon + 0xcfc );

							uint64_t weapon_data = device_t.read<uint64_t>( player_weapon + 0x450 );

							if ( device_t.is_valid( weapon_data ) ) {
								held_weapon.tier = device_t.read<BYTE>( weapon_data + 0x73 );

								uint64_t ftext_ptr = device_t.read<uint64_t>( weapon_data + 0x90 );

								if ( device_t.is_valid( ftext_ptr ) ) {
									uint64_t ftext_data = device_t.read<uint64_t>( ftext_ptr + 0x28 );
									int ftext_length = device_t.read<int>( ftext_ptr + 0x30 );
									if ( ftext_length > 0 && ftext_length < 50 ) {
										wchar_t* ftext_buf = new wchar_t[ftext_length];
										device_t.read( ftext_data, ftext_buf, ftext_length * sizeof( wchar_t ) );
										std::wstring wstr_buf( ftext_buf );
										held_weapon.weapon_name = std::string( wstr_buf.begin( ), wstr_buf.end( ) );

										if ( CurrentPawn == LocalPlayer ) {
											LocalPlayerWeapon = held_weapon.weapon_name;
										}
										delete[] ftext_buf;
									}
								}
							}

							PlayerInfo.WeaponInfo = held_weapon;
						}
					}

					//if ( this->LocalTeam == CurrentState->TeamIndex( ) )
					//	continue;

					if ( CurrentPawn == LocalPlayer )
						continue;

					auto Mesh = CurrentPawn->Mesh( );
					if ( !Mesh ) {
						continue;
					}

					auto BoneArray = Mesh->BoneArray( );
					if ( !BoneArray ) {
						BoneArray = Mesh->BoneArray2( );
						if ( !BoneArray ) continue;
					}

					PlayerInfo.FortnitePawn = CurrentPawn;
					PlayerInfo.Mesh = Mesh;
					PlayerInfo.PlayerState = CurrentPawn->PlayerState( );
					PlayerInfo.BoneArray = BoneArray;

					PlayerInfo.BaseBonePosition = Mesh->GetBoneLocation( BoneArray, bones::HumanHead );

					m_entities.push_back( PlayerInfo );
				}

			}

			this->PlayerArray.clear( );

			for ( auto ent : m_entities ) {
				this->PlayerArray.push_back( ent );
			}

			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}

		return TRUE;
	}
}