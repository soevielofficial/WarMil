#pragma once

class GameOptics
{
public:
	char pad_000[360];
	float crosshair_distance;
	float penetration_distance;
};

class HUD
{
public:
	char pad_0000[641]; //0x0000
	bool air_to_air_indicator;
	bool draw_plane_aim_indicator;
	char pad_0282[9]; //0x0283
	bool penetration_crosshair; //0x028C
	bool unit_glow; //0x028D
	bool gunner_sight_distance; //0x028E
	char pad_028F[69]; //0x028F
	bool show_bombsight; //0x02D4
};