#pragma once
namespace cfg {

	extern bool esp_status;
	extern bool show_reload;
	extern bool show_bots;
	extern bool show_planes;
	extern bool show_bombs;
	extern bool show_rockets;
	extern bool show_offscreen;
	extern bool remove_smokes;
	extern int Bout_type;
	extern int Mout_type;

	extern bool zoom_mod;
	extern bool change_hud;

	extern float off_arrow_size;
	extern float off_radius;
	extern float off_color[];

	extern float zoom_mult;
	extern float alt_mult;
	extern float shadow_mult;

	extern bool block_input;

	constexpr float DEFAULT_ZOOM_MULT = 5.f;
	constexpr float DEFAULT_ALT_MULT = 15.f;
	constexpr float DEFAULT_SHADOW_MULT = 20.f;
}