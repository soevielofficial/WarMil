#include "../header/Misc.h"
#include "../header/Config.h"
#include "../header/Memory.h"
#include "../header/SDK.h"

void Misc() {
	HudChanger();
	ZoomMod();
}

void HudChanger()
{
	UnitList list = *(UnitList*)(memory::address::cGame + memory::offset::UnitList_3);
	GameOptics* addr = *(GameOptics**)(memory::address::cGame + memory::offset::GameOptics);
	if (!list.unitList)
		return;
	HUD* HudInfo = *(HUD**)(memory::address::modulebase + memory::offset::HudInfo);

	HudInfo->penetration_crosshair = true;
	HudInfo->air_to_air_indicator = true;
	HudInfo->draw_plane_aim_indicator = true;
	if (cfg::change_hud)
	{

		addr->crosshair_distance = 2000.f;
		addr->penetration_distance = 2000.f;

		HudInfo->unit_glow = true;

		HudInfo->gunner_sight_distance = true;

		HudInfo->show_bombsight = true;
	}
	else {
		addr->crosshair_distance = 1000.f;
		addr->penetration_distance = 650.f;

		HudInfo->unit_glow = false;

		HudInfo->gunner_sight_distance = false;

		HudInfo->show_bombsight = false;
	}
	return;
}

void ZoomMod()
{
	UnitList list = *(UnitList*)(memory::address::cGame + memory::offset::UnitList_3);
	if (!list.unitList)
		return;
	Player* localplayer = *(Player**)(memory::address::modulebase + memory::offset::LocalPlayer);
	if (localplayer->IsinHangar())
		return;
	if (localplayer->ControlledUnit == NULL or localplayer->ControlledUnit->UnitInfo == NULL)
		return;

	if (cfg::zoom_mod)
	{
		localplayer->ControlledUnit->UnitInfo->ZoomMulti = cfg::zoom_mult;
		localplayer->ControlledUnit->UnitInfo->AlternateMulti = cfg::alt_mult;
		localplayer->ControlledUnit->UnitInfo->ShadowMulti = cfg::shadow_mult;
	}
	else
	{
		localplayer->ControlledUnit->UnitInfo->ZoomMulti = cfg::DEFAULT_ZOOM_MULT;
		localplayer->ControlledUnit->UnitInfo->AlternateMulti = cfg::DEFAULT_ALT_MULT;
		localplayer->ControlledUnit->UnitInfo->ShadowMulti = cfg::DEFAULT_SHADOW_MULT;
	}
	return;
}