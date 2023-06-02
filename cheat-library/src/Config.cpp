#include "../header/Config.h"

bool cfg::esp_status = false;
bool cfg::show_reload = false;
bool cfg::show_bots = false;
bool cfg::show_planes = false;
bool cfg::show_bombs = false;
bool cfg::show_rockets = false;
bool cfg::show_offscreen = false;
bool cfg::remove_smokes = false;
int cfg::Bout_type = 0;
int cfg::Mout_type = 0;

bool cfg::zoom_mod = false;
bool cfg::change_hud = false;

bool cfg::block_input = false;

float cfg::off_arrow_size = 2.f;
float cfg::off_radius = 100.f;
float cfg::off_color[]{ 0.f,0.f,0.f };

float cfg::zoom_mult = 3.f;
float cfg::alt_mult = 12.f;
float cfg::shadow_mult = 20.f;
