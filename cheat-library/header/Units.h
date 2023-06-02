#pragma once
#include "../header/Include.h"
#include "../header/Math.h"
#include <array>
#include <unordered_set>
#include <string>

class Player
{
	enum GuiState : std::uint8_t
	{
		NONE = 0,
		MENU = 1,
		ALIVE = 2,
		DEAD = 3,
		SPEC = 6,
		LIMBO = 8,
		LOADING = 10
	};

public:
	char pad_0000[168]; //0x0000
	char Name[32]; //0x00A8
	char pad_00C8[1048]; //0x00C8
	uint8_t GuiState; //0x04E0
	char pad_04E1[511]; //0x04E1
	class Unit* ControlledUnit; //0x06E0

	bool IsAlive() const noexcept
	{
		return GuiState == GuiState::ALIVE;
	}

	bool IsDead() const noexcept
	{
		return GuiState == GuiState::DEAD;
	}

	bool IsinHangar() const noexcept
	{
		return GuiState == GuiState::MENU;
	}
};


class Unit
{
	enum UnitState : std::uint8_t {
		ALIVE = 0,
		DEAD = 2,
	};

public:
	char pad_0000[504]; //0x0000
	Vector3 BBMin; //0x01F8
	Vector3 BBMax; //0x0204
	char pad_0210[616]; //0x0210
	char* UnitName; //0x0478
	char pad_0480[688]; //0x0480
	uint8_t VisualReload; //0x0730
	char pad_0731[539]; //0x0731
	Matrix3x3 RotationMatrix; //0x094C
	Vector3 Position; //0x0970
	char pad_097C[1424]; //0x097C
	float InvulTimer; //0x0F0C
	char pad_0F10[32]; //0x0F10
	bool Invulnerable; //0x0F30
	char pad_0F31[223]; //0x0F31
	uint8_t UnitState; //0x1010
	char pad_1011[15]; //0x1011
	class Player* PlayerInfo; //0x1020
	char pad_1028[64]; //0x1028
	uint8_t TeamNum; //0x1068
	char pad_1069[15]; //0x1069
	class Info* UnitInfo; //0x1078
	char pad_1080[168]; //0x1080
	class Armory* Weapons; //0x1128



	bool IsAlive() const noexcept
	{
		return UnitState == UnitState::ALIVE;
	}

	bool IsDead() const noexcept
	{
		return UnitState == UnitState::DEAD;
	}
};

class Info
{
public:
	char* unitName; //0x0000
	char* modelName; //0x0008
	char* modelPath; //0x0010
	char* FullName; //0x0018
	char* ShortName; //0x0020
	char pad_0028[8]; //0x0028
	char* unitType; //0x0030
	char pad_0038[428]; //0x0038
	float ZoomMulti; //0x01E4
	float AlternateMulti; //0x01E8
	char pad_01EC[16]; //0x01EC
	float ShadowMulti; //0x01FC


	bool isPlane() {
		static const std::unordered_set<std::string> planeTypes = { "exp_bomber", "exp_assault", "exp_fighter" };
		return planeTypes.find(unitType) != planeTypes.end();
	}

	bool isDummy() {
		static const std::unordered_set<std::string> dummyTypes = { "exp_fortification", "exp_structure", "exp_aaa", "dummy_plane", "exp_bridge", "exp_zero", "exp_ca_ship" };
		return dummyTypes.find(unitType) != dummyTypes.end();
	}
};

class EntList {
public:
	Player* players[64];
};

class Units {
public:
	std::array<Unit*, 500U> units;
};

class UnitList {
public:
	Units* unitList;
};