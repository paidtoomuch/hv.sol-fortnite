#pragma once

#include <dependencies/includes.hpp>
#include <stdint.h>
#include <corecrt_math_defines.h>

#define CURRENT_CLASS reinterpret_cast<uint64_t>(this)
#define DECLARE_MEMBER(type, name, offset) type name() { return device_t.read<type>(CURRENT_CLASS + offset); } 
#define APPLY_MEMBER(type, name, offset) void name( type val ) { device_t.write<type>(CURRENT_CLASS + offset, val); }

namespace m_ue
{
	struct UObject
	{
		DECLARE_MEMBER( FName, NamePrivate, 0x18 );
	};

	struct UActorComponent : public UObject
	{

	};

	struct USceneComponent : public UActorComponent
	{
		DECLARE_MEMBER( FVector, ComponentVelocity, 0x170 );
		DECLARE_MEMBER( FVector, RelativeLocation, 0x128 );
		DECLARE_MEMBER( FVector, RelativeRotation, 0x140 );
	};

	struct UPrimitiveComponent : USceneComponent
	{

	};

	struct UMeshComponent : public UPrimitiveComponent
	{

	};

	struct USkinnedMeshComponent : public UMeshComponent
	{
		DECLARE_MEMBER( UObject*, BoneArray, 0x600 );
		DECLARE_MEMBER( UObject*, BoneArray2, 0x600 + sizeof( TArray ) ); //Yes, there are actually 2 bone arrays in UE4. i forgot the real name of "BoneArray" though xD

		FMatrix GetBoneMatrix( UObject* BoneArray, std::uint32_t Index )
		{
			auto MatrixFirstBone = device_t.read<FTransform>( std::uintptr_t( BoneArray + ( sizeof( FTransform ) * Index ) ) ).ToMatrix( );
			auto MatrixComponentToWorld = device_t.read<FTransform>( std::uintptr_t( this + 0x240 ) ).ToMatrix( );

			return MatrixFirstBone.MatrixMultiplication( MatrixComponentToWorld );
		}

		FVector GetBoneLocation( UObject* BoneArray, std::uint32_t Index )
		{
			auto Matrix = this->GetBoneMatrix( BoneArray, Index );

			return FVector( Matrix.WPlane.X, Matrix.WPlane.Y, Matrix.WPlane.Z );
		}
	};


	struct USkeletalMeshComponent : public USkinnedMeshComponent
	{

	};


	struct AActor : public UObject
	{
		DECLARE_MEMBER( USceneComponent*, RootComponent, 0x198 );

	};

	struct APawn : public AActor
	{
		DECLARE_MEMBER( int32_t, ObjectID, 0x18 );
	};

	struct APlayerState : public UObject
	{
		DECLARE_MEMBER( APawn*, PawnPrivate, 0x308 );
		DECLARE_MEMBER( BYTE, bIsABot, 0x29A );
		DECLARE_MEMBER( int32_t, TeamIndex, 0x10d0 ); // AFortPlayerStateAthena

		bool is_bot( ) //APawn::APlayerState::PawnPrivate
		{
			if ( !CURRENT_CLASS ) return 0;

			auto bIsABot = this->bIsABot( );

			if ( bIsABot >> 3 & 1 )
				return true;
			else
				return false;
		}

		std::string username( bool inlobby )
		{
			int pNameLength; // rsi
			_WORD* pNameBufferPointer;
			int i;      // ecx
			char v25; // al
			int v26;  // er8
			int v29;  // eax

			char16_t* pNameBuffer;

			if ( inlobby ) {
				pNameLength = device_t.read<int>( CURRENT_CLASS + 0x330 );
				auto pNameEncryptedBuffer = device_t.read<uintptr_t>( CURRENT_CLASS + 0x328 );

				pNameBuffer = new char16_t[pNameLength];
				device_t.read_array( pNameEncryptedBuffer, pNameBuffer, pNameLength );
			}
			else {
				uintptr_t pNameStructure = device_t.read<uintptr_t>( CURRENT_CLASS + 0xAE0 );
				pNameLength = device_t.read<int>( pNameStructure + 0x10 );
				if ( pNameLength <= 0 )
					return "";

				pNameBuffer = new char16_t[pNameLength];
				uintptr_t pNameEncryptedBuffer = device_t.read<uintptr_t>( pNameStructure + 0x8 );
				device_t.read_array( pNameEncryptedBuffer, pNameBuffer, pNameLength );
			}

			v25 = pNameLength - 1;
			v26 = 0;
			pNameBufferPointer = ( _WORD* ) pNameBuffer;

			for ( i = ( v25 ) & 3;; *pNameBufferPointer++ += i & 7 ) {
				v29 = pNameLength - 1;
				if ( !( _DWORD ) pNameLength )
					v29 = 0;

				if ( v26 >= v29 )
					break;

				i += 3;
				++v26;
			}

			std::u16string temp_wstring( pNameBuffer );
			delete[] pNameBuffer;
			return std::string( temp_wstring.begin( ), temp_wstring.end( ) );
		}
	};

	struct ACharacter : public APawn
	{
		DECLARE_MEMBER( USkeletalMeshComponent*, Mesh, 0x318 );
		DECLARE_MEMBER( APlayerState*, PlayerState, 0x2B0 );
	};


	struct ftextdata
	{
		char keys[0x30];  //0x0000
		wchar_t* name;          //0x0028 
		__int32 length;         //0x0030 

	};

	struct ftext
	{
		ftextdata* data;
		char keys[0x10];

		auto to_string( ) -> wchar_t*
		{
			if ( this->data ) {
				return data->name;
			}

			return nullptr;
		}
	};


	struct AFGF_Character : public ACharacter
	{

	};

	struct AFortPawn : public AFGF_Character
	{

	};

	struct AFortPlayerPawn : public AFortPawn
	{

	};

	struct AController : public AActor
	{

	};

	struct FText
	{
		DECLARE_MEMBER( uintptr_t, WeaponName, 0x28 );
		DECLARE_MEMBER( int32_t, WeaponLength, 0x30 );
	};

	struct UFortWeaponItemDefinition : public AActor
	{
		DECLARE_MEMBER( FText*, WeaponInformation, 0x90 );

		std::string GetName( )
		{
			int32_t Length = WeaponInformation( )->WeaponLength( );
			uintptr_t WName = WeaponInformation( )->WeaponName( );

			if ( !Length || !WName )
				return _( "None" );

			wchar_t* name = new wchar_t[uintptr_t( Length ) + 1];
			device_t.read_large_array( WName, name, Length ); // FText -> Name    

			std::wstring ws( name );
			delete[] name; // Free the dynamically allocated memory

			return std::string( ws.begin( ), ws.end( ) );
		}

	};

	struct AFortWeapon : public AActor
	{
		DECLARE_MEMBER( UFortWeaponItemDefinition*, WeaponData, 0x450 );
	};

	struct APlayerController : public AController
	{
		DECLARE_MEMBER( APawn*, AcknowledgedPawn, 0x338 );
		DECLARE_MEMBER( bool, IsLocalController, 0x6BC );
		DECLARE_MEMBER( float, LocalPlayerCachedLODDistanceFactor, 0x38c );
		DECLARE_MEMBER( APlayerState*, PlayerState, 0x298 );

		DECLARE_MEMBER( FVector, ViewAngles, 0x520 );
		APPLY_MEMBER( FVector, ViewAngles, 0x520 );

		DECLARE_MEMBER( FVector, VerifyViewAngles, 0x960 );
		APPLY_MEMBER( FVector, VerifyViewAngles, 0x960 );
	};

	struct AFortPlayerPawnAthena : public AFortPlayerPawn
	{
		DECLARE_MEMBER( float, ReviveFromDBNOTime, 0x46B8 );
		DECLARE_MEMBER( APlayerController*, PlayerController, 0x2C8 );
		DECLARE_MEMBER( AFortWeapon*, CurrentWeapon, 0x948 );

		bool IsInScreen( Vector2 HeadPosDol, int Width, int Height )
		{
			if ( ( ( HeadPosDol.x <= 0 || HeadPosDol.x > Width ) && ( HeadPosDol.y <= 0 || HeadPosDol.y > Height ) ) || ( ( HeadPosDol.x <= 0 || HeadPosDol.x > Width ) || ( HeadPosDol.y <= 0 || HeadPosDol.y > Height ) ) ) {
				return false;
			}
			else {
				return true;
			}
		}
	};

	struct USceneViewState : public UObject
	{
		DECLARE_MEMBER( double, YawSine, 0x8f0 );
		DECLARE_MEMBER( double, YawCosine, 0x910 );
		DECLARE_MEMBER( double, PitchSine, 0x9c0 );
	};


	struct ULocalPlayer : public UObject
	{
		DECLARE_MEMBER( APlayerController*, PlayerController, 0xC0 );
		DECLARE_MEMBER( TArray, ViewStates, 0xd0 );

		USceneViewState* GetViewState( )
		{
			return reinterpret_cast< USceneViewState* >( this->ViewStates( ).Get( 1 ) );
		}
	};

	struct APlayerCameraManager : public AActor
	{
		static FVector GetCameraLocation( UObject* ViewLocationRenderedLastFrame )
		{
			return device_t.read<FVector>( std::uintptr_t( ViewLocationRenderedLastFrame ) );
		}

		static FVector GetCameraRotation( uintptr_t ViewLocationRenderedLastFrame )
		{
			FVector ReturnValue;

			struct FNRotation
			{
				double a; //0x0000
				char pad_0008[24]; //0x0008
				double b; //0x0020
				char pad_0028[424]; //0x0028
				double c; //0x01D0
			}tpmrotation;

			tpmrotation.a = device_t.read<double>( ViewLocationRenderedLastFrame );
			tpmrotation.b = device_t.read<double>( ViewLocationRenderedLastFrame + 0x20 );
			tpmrotation.c = device_t.read<double>( ViewLocationRenderedLastFrame + 0x1d0 );

			ReturnValue.X = asin( tpmrotation.c ) * ( 180.0 / M_PI );
			ReturnValue.Y = ( ( atan2( tpmrotation.a * -1, tpmrotation.b ) * ( 180.0 / M_PI ) ) * -1 ) * -1;

			return ReturnValue;
		}

		static float GetFOVAngle( APlayerController* Controller )
		{
			auto LocalPlayerCachedLODDistanceFactor = Controller->LocalPlayerCachedLODDistanceFactor( );

			return LocalPlayerCachedLODDistanceFactor * 90.f;
		}
	};
	
	struct AWorldSettings : public UObject
	{
		DECLARE_MEMBER( float, WorldGravityZ, 0x318 );
	};

	struct ULevel : public UObject
	{
		DECLARE_MEMBER( TArray, Actors, 0x98 );
		DECLARE_MEMBER( AWorldSettings*, WorldSettingsClass, 0x2a0 );
	};

	struct UGameInstance : public UObject
	{
		DECLARE_MEMBER( TArray, LocalPlayers, 0x38 );

		ULocalPlayer* GetLocalPlayer( )
		{
			return reinterpret_cast< ULocalPlayer* >( this->LocalPlayers( ).Get( 0 ) );
		}
	};

	struct AGameStateBase : public UObject
	{
		DECLARE_MEMBER( TArray, PlayerArray, 0x2A8 );
		DECLARE_MEMBER( float, ServerWorldTimeSecondsDelta, 0x2C8 );
	};

	struct UWorld : public UObject
	{
		DECLARE_MEMBER( ULevel*, PersistentLevel, 0x30 );
		DECLARE_MEMBER( UObject*, ViewLocationsRenderedLastFrame, 0x110 ); //Actually a TArray
		DECLARE_MEMBER( UObject*, ViewLocationsRenderedNextFrame, 0x120 ); //Actually a TArray
		DECLARE_MEMBER( UGameInstance*, OwningGameInstance, 0x1b8 );
		DECLARE_MEMBER( AGameStateBase*, GameState, 0x158 );

		static UWorld* Get( )
		{
			return device_t.read< UWorld* >( device_t.m_base + 0xEE2F8E8 );
		}
	};

	struct APlayerClass
	{

	};

}

