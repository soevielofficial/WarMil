#pragma once
#include <array>
#include "../header/Math.h"


//Classes for bombs and rockets - projectiles
class Projectile
{
public:
	char pad_0000[456]; //0x0000
	char* Name; //0x01C8
	char pad_01D0[224]; //0x01D0
	Vector3 Position; //0x02B0
	char pad_02BC[316]; //0x02BC
	class Unit* OwnerUnit; //0x03F8
	class Player* OwnerPlayer; //0x0400
	char pad_0408[472]; //0x0408
	float Lifetime; //0x05E0
	char pad_05E4[8]; //0x05E4
	float ExplodeTime; //0x05EC
	char pad_05F0[104]; //0x05F0
	float BombDelay; //0x0658


	float getExpodeTime() {
		return ExplodeTime <= 0 ? BombDelay : (BombDelay - ExplodeTime);
	}
};


class Projectiles {
public:
	std::array<Projectile*, 100U> projectiles;
};

class ProjectileList {
public:
	Projectiles* projectileList;
};
