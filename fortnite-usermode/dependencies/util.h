#pragma once
#include <corecrt_math_defines.h>

struct RGB
{
	float R, G, B;
};

static std::string ReadGetNameFromFName( int key )
{
	uint32_t ChunkOffset = ( uint32_t ) ( ( int ) ( key ) >> 16 );
	uint16_t NameOffset = ( uint16_t ) key;

	uint64_t NamePoolChunk = device_t.read<uint64_t>( device_t.m_base + 0xEB03740 + ( 8 * ChunkOffset ) + 16 ) + ( unsigned int ) ( 4 * NameOffset ); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	uint16_t nameEntry = device_t.read<uint16_t>( NamePoolChunk );

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ( ( uint32_t ) nameLength ) {
		for ( int x = 0; x < nameLength; ++x ) {
			buff[x] = device_t.read<char>( NamePoolChunk + 4 + x );
		}

		char* v2; // rdi
		int v4; // ebx
		int v5; // edx
		__int64 result; // rax
		int v7; // ecx
		unsigned int v8; // eax

		char* a2 = buff;
		unsigned int a1 = nameLength;

		v2 = a2;
		memcpy( a2, &a1 + 2, ( unsigned __int64 ) *&a1 >> 6 );
		v4 = *&a1 >> 6;
		v5 = 0;
		result = 16i64;
		if ( v4 ) {
			do {
				++v2;
				v7 = v5++ | 0xB000;
				v8 = v7 + result;
				LOBYTE( v7 ) = v8 ^ ~*( v2 - 1 );
				result = v8 >> 2;
				*( v2 - 1 ) = v7;
			} while ( v5 < v4 );
		}

		//char* v2; // rdi
		//int v4; // ebx
		//unsigned int v5; // eax
		//int v6; // er8
		//unsigned int v7; // ecx
		//char v8; // dl

		//v2 = buff;
		//v4 = nameLength;
		//v5 = device_t.read<DWORD>( device_t.m_base + 0xED83F44 ) >> 5;
		//v6 = 0;
		//if ( v4 ) {
		//	do {
		//		v7 = *v2++;
		//		v8 = v5 ^ ( 16 * v7 ) ^ ( v5 ^ ( v7 >> 4 ) ) & 0xF;
		//		v5 += 2 * v6++;
		//		*( v2 - 1 ) = v8;
		//	} while ( v6 < v4 );
		//}

		buff[nameLength] = '\0';
		return std::string( buff );
	}
	else {
		return "error";
	}
}

namespace utl
{
	inline DWORD get_process_id(
		const LPCWSTR process_name )
	{
		HANDLE handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
		DWORD procID = NULL;

		if ( handle == INVALID_HANDLE_VALUE )
			return procID;

		PROCESSENTRY32W entry = { 0 };
		entry.dwSize = sizeof( PROCESSENTRY32W );

		if ( Process32FirstW( handle, &entry ) ) {
			if ( !_wcsicmp( process_name, entry.szExeFile ) )
			{
				procID = entry.th32ProcessID;
			}
			else while ( Process32NextW( handle, &entry ) ) {
				if ( !_wcsicmp( process_name, entry.szExeFile ) ) {
					procID = entry.th32ProcessID;
				}
			}
		}

		CloseHandle( handle );
		return procID;
	}
}

struct FVector
{
	FVector( ) : X( ), Y( ), Z( ) {}
	FVector( double X, double Y, double Z ) : X( X ), Y( Y ), Z( Z ) {}

	FVector operator		+ ( const FVector& other ) const { return { this->X + other.X, this->Y + other.Y, this->Z + other.Z }; }
	FVector operator		- ( const FVector& other ) const { return { this->X - other.X, this->Y - other.Y, this->Z - other.Z }; }
	FVector operator		* ( double offset ) const { return { this->X * offset, this->Y * offset, this->Z * offset }; }
	FVector operator		/ ( double offset ) const { return { this->X / offset, this->Y / offset, this->Z / offset }; }

	FVector& operator		*= ( const double other ) { this->X *= other; this->Y *= other; this->Z *= other; return *this; }
	FVector& operator		/= ( const double other ) { this->X /= other; this->Y /= other; this->Z /= other; return *this; }

	FVector& operator		=  ( const FVector& other ) { this->X = other.X; this->Y = other.Y; this->Z = other.Z; return *this; }
	FVector& operator		+= ( const FVector& other ) { this->X += other.X; this->Y += other.Y; this->Z += other.Z; return *this; }
	FVector& operator		-= ( const FVector& other ) { this->X -= other.X; this->Y -= other.Y; this->Z -= other.Z; return *this; }
	FVector& operator		*= ( const FVector& other ) { this->X *= other.X; this->Y *= other.Y; this->Z *= other.Z; return *this; }
	FVector& operator		/= ( const FVector& other ) { this->X /= other.X; this->Y /= other.Y; this->Z /= other.Z; return *this; }

	operator bool( ) { return bool( this->X || this->Y || this->Z ); }
	friend bool operator	== ( const FVector& a, const FVector& b ) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
	friend bool operator	!= ( const FVector& a, const FVector& b ) { return !( a == b ); }

	float Dot( const FVector& vector ) { return X * vector.X + Y * vector.Y + Z * vector.Z; }
	double Distance( FVector v ) { return double( sqrtf( powf( v.X - X, 2.0 ) + powf( v.Y - Y, 2.0 ) + powf( v.Z - Z, 2.0 ) ) ); }

	double X, Y, Z;
};

struct Vector2
{
	Vector2( ) : x( 0 ), y( 0 ) {}
	Vector2( float x, float y ) : x( x ), y( y ) {}

	Vector2 operator		+ ( const Vector2& other ) const { return { this->x + other.x, this->y + other.y }; }
	Vector2 operator		- ( const Vector2& other ) const { return { this->x - other.x, this->y - other.y }; }
	Vector2 operator		* ( float offset ) const { return { this->x * offset, this->y * offset }; }
	Vector2 operator		/ ( float offset ) const { return { this->x / offset, this->y / offset }; }

	Vector2& operator		*= ( const float other ) { this->x *= other; this->y *= other; return *this; }
	Vector2& operator		/= ( const float other ) { this->x /= other; this->y /= other; return *this; }

	Vector2& operator		=  ( const Vector2& other ) { this->x = other.x; this->y = other.y; return *this; }
	Vector2& operator		+= ( const Vector2& other ) { this->x += other.x; this->y += other.y; return *this; }
	Vector2& operator		-= ( const Vector2& other ) { this->x -= other.x; this->y -= other.y; return *this; }
	Vector2& operator		*= ( const Vector2& other ) { this->x *= other.x; this->y *= other.y; return *this; }
	Vector2& operator		/= ( const Vector2& other ) { this->x /= other.x; this->y /= other.y; return *this; }

	operator bool( ) { return bool( this->x || this->y ); }
	friend bool operator	==( const Vector2& a, const Vector2& b ) { return a.x == b.x && a.y == b.y; }
	friend bool operator	!=( const Vector2& a, const Vector2& b ) { return !( a == b ); }

	float Distance( Vector2 v ) {
		return float( sqrt( pow( v.x - x, 2.0 ) + pow( v.y - y, 2.0 ) ) );
	}

	float x, y;
};


struct FMinimalViewInfo
{
	FMinimalViewInfo( ) : Location( ), Rotation( ), FOV( FOV ) {}
	FMinimalViewInfo( FVector Location, FVector Rotation, float FOV ) : Location( Location ), Rotation( Rotation ), FOV( FOV ) {}

	FVector Location, Rotation;
	float FOV;
};

struct FPlane : public FVector
{
	double W;
};

struct FMatrix
{
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix MatrixMultiplication( FMatrix Other )
	{
		FMatrix Copy = {};

		Copy.WPlane.X = this->WPlane.X * Other.XPlane.X + this->WPlane.Y * Other.YPlane.X + this->WPlane.Z * Other.ZPlane.X + this->WPlane.W * Other.WPlane.X;
		Copy.WPlane.Y = this->WPlane.X * Other.XPlane.Y + this->WPlane.Y * Other.YPlane.Y + this->WPlane.Z * Other.ZPlane.Y + this->WPlane.W * Other.WPlane.Y;
		Copy.WPlane.Z = this->WPlane.X * Other.XPlane.Z + this->WPlane.Y * Other.YPlane.Z + this->WPlane.Z * Other.ZPlane.Z + this->WPlane.W * Other.WPlane.Z;
		Copy.WPlane.W = this->WPlane.X * Other.XPlane.W + this->WPlane.Y * Other.YPlane.W + this->WPlane.Z * Other.ZPlane.W + this->WPlane.W * Other.WPlane.W;

		return Copy;
	}

	static FMatrix RotatorToMatrix( FVector Rotation )
	{
		auto ReturnValue = FMatrix( );

		auto Pitch = ( Rotation.X * 3.14159265358979323846 / 180 );
		auto Yaw = ( Rotation.Y * 3.14159265358979323846 / 180 );
		auto Roll = ( Rotation.Z * 3.14159265358979323846 / 180 );

		auto sp = sin( Pitch );
		auto cp = cos( Pitch );
		auto sy = sin( Yaw );
		auto cy = cos( Yaw );
		auto sr = sin( Roll );
		auto cr = cos( Roll );

		ReturnValue.XPlane.X = cp * cy;
		ReturnValue.XPlane.Y = cp * sy;
		ReturnValue.XPlane.Z = sp;

		ReturnValue.YPlane.X = sr * sp * cy - cr * sy;
		ReturnValue.YPlane.Y = sr * sp * sy + cr * cy;
		ReturnValue.YPlane.Z = -sr * cp;

		ReturnValue.ZPlane.X = -( cr * sp * cy + sr * sy );
		ReturnValue.ZPlane.Y = cy * sr - cr * sp * sy;
		ReturnValue.ZPlane.Z = cr * cp;

		ReturnValue.WPlane.W = 0x1;

		return ReturnValue;
	}
};

struct FTransform
{
	FPlane Rotation;
	FVector Translation;
	char pad_0[8];
	FVector Scale3D;
	char pad_1[4];

	FMatrix ToMatrix( )
	{
		auto ReturnValue = FMatrix( );

		auto x2 = this->Rotation.X * 2;
		auto y2 = this->Rotation.Y * 2;
		auto z2 = this->Rotation.Z * 2;

		auto xx2 = this->Rotation.X * x2;
		auto yy2 = this->Rotation.Y * y2;
		auto zz2 = this->Rotation.Z * z2;

		auto yz2 = this->Rotation.Y * z2;
		auto wx2 = this->Rotation.W * x2;

		auto xy2 = this->Rotation.X * y2;
		auto wz2 = this->Rotation.W * z2;

		auto xz2 = this->Rotation.X * z2;
		auto wy2 = this->Rotation.W * y2;

		ReturnValue.XPlane.X = ( 1.0 - ( yy2 + zz2 ) ) * this->Scale3D.X;
		ReturnValue.XPlane.Y = ( xy2 + wz2 ) * this->Scale3D.X;
		ReturnValue.XPlane.Z = ( xz2 - wy2 ) * this->Scale3D.X;

		ReturnValue.YPlane.X = ( xy2 - wz2 ) * this->Scale3D.Y;
		ReturnValue.YPlane.Y = ( 1.0 - ( xx2 + zz2 ) ) * this->Scale3D.Y;
		ReturnValue.YPlane.Z = ( yz2 + wx2 ) * this->Scale3D.Y;

		ReturnValue.ZPlane.X = ( xz2 + wy2 ) * this->Scale3D.Z;
		ReturnValue.ZPlane.Y = ( yz2 - wx2 ) * this->Scale3D.Z;
		ReturnValue.ZPlane.Z = ( 1.0 - ( xx2 + yy2 ) ) * this->Scale3D.Z;

		ReturnValue.WPlane.X = this->Translation.X;
		ReturnValue.WPlane.Y = this->Translation.Y;
		ReturnValue.WPlane.Z = this->Translation.Z;

		ReturnValue.WPlane.W = 1.0;

		return ReturnValue;
	}
};


struct FName
{
	std::uint32_t Index;
};

struct TArray
{
	std::uintptr_t Array;
	std::uint32_t Count;
	std::uint32_t MaxCount;

	std::uintptr_t Get( std::uint32_t Index )
	{
		return device_t.read<std::uintptr_t>( Array + ( Index * 0x8 ) );
	}
};

namespace customs
{
	inline auto in_rect( double x, double y, double radius, double coord_x, double coord_y ) -> bool
	{
		return coord_x >= x - radius && coord_x <= x + radius &&
			coord_y >= y - radius && coord_y <= y + radius;
	}

	inline auto in_circle( double x, double y, double radius, double coord_x, double coord_y ) -> bool
	{
		if ( in_rect( x, coord_y, radius, x, coord_y ) ) {
			double dx = x - coord_x;
			double dy = y - coord_y;
			dx *= dx;
			dy *= dy;
			double distance_squared = dx + dy;
			double radius_squared = radius * radius;
			return distance_squared <= radius_squared;
		} return false;
	}


	inline float degree_to_radian( float degree )
	{
		return degree * ( M_PI / 180 );
	}

	inline void angle_rotation( const FVector& angles, FVector* forward )
	{
		float	sp, sy, cp, cy;

		sy = sin( degree_to_radian( angles.Y ) );
		cy = cos( degree_to_radian( angles.Y ) );

		sp = sin( degree_to_radian( angles.X ) );
		cp = cos( degree_to_radian( angles.X ) );

		forward->X = cp * cy;
		forward->Y = cp * sy;
		forward->Z = -sp;
	}
}

