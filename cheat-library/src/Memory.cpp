#include "../header/Memory.h"
#include "../header/SigScanner.h"

namespace memory
{
	Vector2 scrsize;

	namespace offset
	{
		uintptr_t cGame;
		uintptr_t LocalPlayer;
		uintptr_t HudInfo;
		uintptr_t ScreenWidth;
		uintptr_t IsScoping;
		uintptr_t mat_addr;
		uintptr_t BombArray;
		uintptr_t RocketArray;
	}

	namespace address
	{
		uintptr_t cGame;
		uintptr_t modulebase;
	}
}

void memory::InitMemory() {
	address::modulebase = (uintptr_t)GetModuleHandle(NULL);
	std::vector<uintptr_t> offsets = GetOffsets(sigs::signatures);
	int scrW = *(int*)(address::modulebase + offsets[3]);
	int scrH = *(int*)(address::modulebase + offsets[3] + 0x4);

	offset::cGame = offsets[0];
	offset::LocalPlayer = offsets[1];
	offset::HudInfo = offsets[2];
	offset::ScreenWidth = offsets[3];
	offset::IsScoping = offsets[4];
	offset::mat_addr = offsets[5];
	offset::BombArray = offsets[6];
	offset::RocketArray = offsets[7];

	scrsize = { (float)scrW, (float)scrH };

	address::cGame = *(uintptr_t*)(address::modulebase + offset::cGame);
}

