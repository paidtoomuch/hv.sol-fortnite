#pragma once

namespace m_cfg
{
	inline bool m_show_menu = false;
	inline bool m_debug = false;
	inline bool m_streamproof = false;
	inline bool m_crosshair = true;

	inline int proof = 0;

	namespace aim {
		inline bool m_memory = true;
		inline bool m_draw_fov = true;
		inline bool m_prediction = true;
		inline float m_smoothness = 2.0f;
		inline float m_fov = 45.0f;
		inline int m_target_bone = 0;
		inline float m_aimbot_distance = 450.0f;
		inline const char* m_bone_names[]{ ("head"), ("neck"), ("torso"), ("left arm"), ("right arm"), ("left leg"), ("right leg") };
	}
	namespace player {
		inline bool m_distance = true;
		inline bool m_skeleton = true;
		inline bool m_name = true;
		inline bool m_team = false;
		inline bool m_box = true;
		inline bool m_view_angle = true;
		inline bool m_health_bar = true;
		inline bool m_shield_bar = true;
		inline bool m_current_gun = true;
		inline bool m_reloading_status = true;
		inline bool m_chams = true;

		inline float m_render_distance = 450.0f;
		inline float m_view_angle_distance = 250.0f;
	}

	namespace radar
	{
		inline bool m_radar = true;
		inline bool m_draw_team = false;
		inline int x_axis = 0;
		inline int y_axis = 400;
		inline int m_radar_width = 250;
		inline int m_radar_height = 250;
		inline int m_radar_distance = 400;
	}

	inline float m_esp_render_distance = 175.0f;
}
