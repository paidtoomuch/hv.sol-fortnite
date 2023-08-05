#pragma once 
#include <dependencies/includes.hpp>
#include <iostream>
#include <sstream>

using namespace m_ue;

void m_ue::overwolf_t::text( const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center ) {
	constexpr float fStrokeVal1 = 1.0f;
	uint32_t EdgeColor = 0xFF000000;

	float Edge_a = ( EdgeColor >> 24 ) & 0xff;
	float Edge_r = ( EdgeColor >> 16 ) & 0xff;
	float Edge_g = ( EdgeColor >> 8 ) & 0xff;
	float Edge_b = ( EdgeColor ) & 0xff;
	std::stringstream steam( text );
	std::string line;
	float y = 0.0f;
	int i = 0;
	while ( std::getline( steam, line ) )
	{
		ImVec2 textSize = this->m_font->CalcTextSizeA( size, FLT_MAX, 0.0f, line.c_str( ) );
		if ( center )
		{
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x - textSize.x / 2.0f ) - fStrokeVal1, pos.y + textSize.y * i ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x - textSize.x / 2.0f ) + fStrokeVal1, pos.y + textSize.y * i ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x - textSize.x / 2.0f ), ( pos.y + textSize.y * i ) - fStrokeVal1 ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x - textSize.x / 2.0f ), ( pos.y + textSize.y * i ) + fStrokeVal1 ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), color, line.c_str( ) );
		}
		else
		{
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x ) - fStrokeVal1, ( pos.y + textSize.y * i ) ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x ) + fStrokeVal1, ( pos.y + textSize.y * i ) ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x ), ( pos.y + textSize.y * i ) - fStrokeVal1 ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( ( pos.x ), ( pos.y + textSize.y * i ) + fStrokeVal1 ), ImGui::GetColorU32( ImVec4( Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255 ) ), line.c_str( ) );
			ImGui::GetBackgroundDrawList( )->AddText( this->m_font, size, ImVec2( pos.x, pos.y + textSize.y * i ), color, line.c_str( ) );
		}
		y = pos.y + textSize.y * ( i + 1 );
		i++;
	}
}

void m_ue::overwolf_t::line( const ImVec2& from, const ImVec2& to, uint32_t color, float thickness ) {
	ImGui::GetBackgroundDrawList( )->AddLine( from, to, color, thickness );
}

bool m_ue::overwolf_t::world_to_screen( FVector world_point, Vector2* to_screen_point ) {
	auto Matrix = FMatrix::RotatorToMatrix( this->m_camera_information.Rotation );

	auto XAxis = FVector( Matrix.XPlane.X, Matrix.XPlane.Y, Matrix.XPlane.Z );
	auto YAxis = FVector( Matrix.YPlane.X, Matrix.YPlane.Y, Matrix.YPlane.Z );
	auto ZAxis = FVector( Matrix.ZPlane.X, Matrix.ZPlane.Y, Matrix.ZPlane.Z );

	auto DeltaCoordinates = world_point - this->m_camera_information.Location;

	auto Transform = FVector( DeltaCoordinates.Dot( YAxis ), DeltaCoordinates.Dot( ZAxis ), DeltaCoordinates.Dot( XAxis ) );
	if ( Transform.Z < 1.f ) {
		Transform.Z = 1.f;
	}

	to_screen_point->x = ( this->m_width_center ) + Transform.X * ( ( ( this->m_width_center ) / tanf( this->m_camera_information.FOV * ( float ) M_PI / 360.f ) ) ) / Transform.Z;
	to_screen_point->y = ( this->m_height_center ) - Transform.Y * ( ( ( this->m_width_center ) / tanf( this->m_camera_information.FOV * ( float ) M_PI / 360.f ) ) ) / Transform.Z;


	return true;
}
