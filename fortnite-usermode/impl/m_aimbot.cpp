#pragma once 
#include <dependencies/includes.hpp>

using namespace m_ue;

using namespace m_cfg;

APlayerInformation* m_ue::overwolf_t::find_target( )
{
    APlayerInformation* AimbotTarget = nullptr;
    float ofov = ( m_cfg::aim::m_fov * this->m_width_center / this->m_camera_information.FOV ) / 2;

    for ( auto& PawnInfo : this->PlayerArray ) {
        auto Pawn = PawnInfo.FortnitePawn;
        auto BoneArray = PawnInfo.BoneArray;
        auto BaseBonePosition = PawnInfo.BaseBonePosition;

        auto Distance = m_camera_information.Location.Distance( BaseBonePosition ) * 0.01;

        auto PrevPawn = AimbotTarget ? AimbotTarget->FortnitePawn : nullptr;

        if ( PrevPawn ) {
            auto TargetLocation = PrevPawn->RootComponent( )->RelativeLocation( );
            auto TargetDistance = m_camera_information.Location.Distance( TargetLocation ) * 0.01;

            if ( Distance >= TargetDistance )
                continue;
        }

        float nfov = 0;
        Vector2 out;
        if ( this->world_to_screen( BaseBonePosition, &out ) ) {
            nfov = ( fabs( out.x - this->m_width_center ) + fabs( out.y - this->m_height_center ) );
        }

        if ( nfov != 0 && nfov < ofov ) {
            ofov = nfov;
            AimbotTarget = &PawnInfo;
        }
    }

    return AimbotTarget;
}

auto m_ue::overwolf_t::aimbot_queue( ) -> bool
{
    for ( ;; ) {
        if ( !this->GWorld || !m_cfg::aim::m_memory /*|| !InLobby*/ ) {
            continue;
        }

        auto PawnInfo = this->find_target( );
        auto Controller = this->LocalPlayerController;

        if ( PawnInfo && GetAsyncKeyState( VK_RBUTTON ) & 0x8000 ) {
            auto Pawn = PawnInfo->FortnitePawn;
            auto Mesh = PawnInfo->Mesh;
            auto BoneArray = PawnInfo->BoneArray;
            auto BaseBonePosition = PawnInfo->BaseBonePosition;
            auto Velocity = Pawn->RootComponent( )->ComponentVelocity( );

            float projectileSpeed = 0;
            float projectileGravityScale = 0;

            auto LocalPlayerPawn = reinterpret_cast< AFortPlayerPawnAthena* >( this->LocalPlayer );

            int target_bone = bones::HumanChest;

            switch ( m_cfg::aim::m_target_bone ) {
                case 0:
                {
                    target_bone = bones::HumanHead;
                    break;
                }

                case 1:
                {
                    target_bone = bones::HumanNeck;
                    break;
                }

                case 2:
                {
                    target_bone = bones::HumanChest;
                    break;
                }

                case 3:
                {
                    target_bone = bones::HumanLForearm2;
                    break;
                }

                case 4:
                {
                    target_bone = bones::HumanRForearm2;
                    break;
                }

                case 5:
                {
                    target_bone = bones::HumanLCalf;
                    break;
                }
                case 6:
                {
                    target_bone = bones::HumanRCalf;
                    break;
                }
            }
            if ( m_cfg::aim::m_prediction ) {
                if ( strstr( LocalPlayerWeapon.c_str( ), ( "Dragon's Breath Sniper" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "Storm Scout" ) ) ||
                    strstr( LocalPlayerWeapon.c_str( ), ( "Storm Scout Sniper Rifle" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "Hunting Rifle" ) ) ||
                    strstr( LocalPlayerWeapon.c_str( ), ( "Explosive Repeater Rifle" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "Bolt-Action Sniper Rifle" ) ) ||
                    strstr( LocalPlayerWeapon.c_str( ), ( "Suppressed Sniper Rifle" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "Lever Action Rifle" ) ) ||
                    strstr( LocalPlayerWeapon.c_str( ), ( "Boom Sniper Rifle" ) ) ) {

                    projectileSpeed = 30000.f;
                    projectileGravityScale = 0.12f;
                }
                else if ( strstr( LocalPlayerWeapon.c_str( ), ( "Heavy Sniper Rifle" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "Hunter Bolt-Action Sniper" ) ) ) {
                    projectileSpeed = 45000.f;
                    projectileGravityScale = 0.12f;
                }
                else if ( strstr( LocalPlayerWeapon.c_str( ), ( "Cobra DMR" ) ) || strstr( LocalPlayerWeapon.c_str( ), ( "DMR" ) )|| strstr( LocalPlayerWeapon.c_str( ), ( "Thermal DMR" ) ) ) {
                    projectileSpeed = 53000.f;
                    projectileGravityScale = 0.15f;
                }
                else if ( strstr( LocalPlayerWeapon.c_str( ), ( "Automatic Sniper Rifle" ) ) ) {
                    projectileSpeed = 50000.f;
                    projectileGravityScale = 0.12f;
                }
                else {
                    projectileSpeed = 0;
                    projectileGravityScale = 0;
                }
            }

            if ( projectileSpeed ) {
                target_bone = bones::HumanHead;
            }

            FVector BonePosition = Mesh->GetBoneLocation( BoneArray, target_bone );

            auto Distance = m_camera_information.Location.Distance( BonePosition );

            if ( projectileSpeed ) {
                BonePosition = PredictLocation( BonePosition, Velocity, projectileSpeed, projectileGravityScale, Distance ); //dmr
            }

            Vector2 HeadScreen;

            if ( world_to_screen( BonePosition, &HeadScreen ) ) {
                float x = HeadScreen.x;
                float y = HeadScreen.y;

                float Smoothness = m_cfg::aim::m_smoothness;
                float Snappiness = 0.5f;

                float TargetX = 0;
                float TargetY = 0;

                float ScreenCenterX = this->m_width_center;
                float ScreenCenterY = this->m_height_center;


                if ( x != 0 ) {
                    if ( x > ScreenCenterX ) {
                        TargetX = -( ScreenCenterX - x );
                        TargetX /= Smoothness;
                        if ( TargetX + ScreenCenterX > ScreenCenterX * 2 ) TargetX = 0;
                    }

                    if ( x < ScreenCenterX ) {
                        TargetX = x - ScreenCenterX;
                        TargetX /= Smoothness;
                        if ( TargetX + ScreenCenterX < 0 ) TargetX = 0;
                    }
                }

                if ( y != 0 ) {
                    if ( y > ScreenCenterY ) {
                        TargetY = -( ScreenCenterY - y );
                        TargetY /= Smoothness;
                        if ( TargetY + ScreenCenterY > ScreenCenterY * 2 ) TargetY = 0;
                    }

                    if ( y < ScreenCenterY ) {
                        TargetY = y - ScreenCenterY;
                        TargetY /= Smoothness;
                        if ( TargetY + ScreenCenterY < 0 ) TargetY = 0;
                    }
                }

                FVector Angles;
                FVector CurrentAngles = Controller->ViewAngles( );

                float NewTargetY;
                float NewTargetX;

                if ( m_camera_information.FOV > 60 ) {
                    NewTargetX = CurrentAngles.Y + ( TargetX / 5 );
                    NewTargetY = CurrentAngles.X - ( TargetY / 5 );
                }
                else {
                    NewTargetX = CurrentAngles.Y + ( TargetX / 10 );
                    NewTargetY = CurrentAngles.X - ( TargetY / 10 );
                }

                NewTargetY = ( 1 - Snappiness ) * CurrentAngles.X + Snappiness * NewTargetY;
                NewTargetX = ( 1 - Snappiness ) * CurrentAngles.Y + Snappiness * NewTargetX;


                //float RandomNoiseX = static_cast< float >( rand( ) % 5 ) / 10.0f - 0.25f;
                //float RandomNoiseY = static_cast< float >( rand( ) % 5 ) / 10.0f - 0.25f;

                //NewTargetX += RandomNoiseX;
                //NewTargetY += RandomNoiseY;

                Angles = FVector{ NewTargetY, NewTargetX, 0 }; // 0x960

                Controller->ViewAngles( Angles );
                //Controller->VerifyViewAngles( Angles );
            }
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}