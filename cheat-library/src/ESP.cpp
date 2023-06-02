#include "../header/ESP.h"
#include "../header/SDK.h"
#include "../header/Menu.h" // for fonts
#include <iostream>
#include <iomanip>   
#include <sstream>
#include <array>
#include <format>

bool WorldToScreen(const Vector3& in, Vector3& out)
{
	const uintptr_t mat_addr = memory::address::modulebase + memory::offset::mat_addr;
	const ViewMatrix& mat = *reinterpret_cast<ViewMatrix*>(mat_addr);

	float width = mat[0][3] * in.x + mat[1][3] * in.y + mat[2][3] * in.z + mat[3][3];

	bool visible = width >= 0.1f;
	if (!visible)
		return false;
	else
		width = 1.0f / width;

	float x = in.x * mat[0][0] + in.y * mat[1][0] + in.z * mat[2][0] + mat[3][0];
	float y = in.x * mat[0][1] + in.y * mat[1][1] + in.z * mat[2][1] + mat[3][1];
	float z = in.x * mat[0][2] + in.y * mat[1][2] + in.z * mat[2][2] + mat[3][2];

	float nx = x * width;
	float ny = y * width;
	float nz = z * width;


	out.x = (memory::scrsize.x / 2 * nx) + (nx + memory::scrsize.x / 2);
	out.y = -(memory::scrsize.y / 2 * ny) + (ny + memory::scrsize.y / 2);
	out.z = nz;

	return true;
}

void DrawBox(Matrix3x3 rotation, Vector3 bbmin, Vector3 bbmax, Vector3 position, bool Invulnerable, Vector3& bbcenter)
{
	Vector3 ax[6];
	ax[0] = Vector3{ rotation[0][0], rotation[0][1], rotation[0][2] }.Scale(bbmin.x);
	ax[1] = Vector3{ rotation[1][0], rotation[1][1], rotation[1][2] }.Scale(bbmin.y);
	ax[2] = Vector3{ rotation[2][0], rotation[2][1], rotation[2][2] }.Scale(bbmin.z);
	ax[3] = Vector3{ rotation[0][0], rotation[0][1], rotation[0][2] }.Scale(bbmax.x);
	ax[4] = Vector3{ rotation[1][0], rotation[1][1], rotation[1][2] }.Scale(bbmax.y);
	ax[5] = Vector3{ rotation[2][0], rotation[2][1], rotation[2][2] }.Scale(bbmax.z);

	Vector3 temp[6];
	temp[0] = position + ax[2];
	temp[1] = position + ax[5];
	temp[2] = temp[0] + ax[3];
	temp[3] = temp[1] + ax[3];
	temp[4] = temp[0] + ax[0];
	temp[5] = temp[1] + ax[0];

	Vector3 v[8];
	v[0] = temp[2] + ax[1];
	v[1] = temp[2] + ax[4];
	v[2] = temp[3] + ax[4];
	v[3] = temp[3] + ax[1];
	v[4] = temp[4] + ax[1];
	v[5] = temp[4] + ax[4];
	v[6] = temp[5] + ax[4];
	v[7] = temp[5] + ax[1];

	const auto draw = ImGui::GetBackgroundDrawList();

	ImColor color1 = ImColor(255, 0, 0); //Front color
	ImColor color2 = ImColor(255, 255, 255); //Other


	if (Invulnerable)
		color2 = ImColor(255, 0, 0);

	Vector3 p1, p2;
	for (int i = 0; i < 4; i++)
	{

		if (WorldToScreen(v[i], p1) && WorldToScreen(v[(i + 1) & 3], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color1, 2.f);

		if (WorldToScreen(v[4 + i], p1) && WorldToScreen(v[4 + ((i + 1) & 3)], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color2, 2.f);

		if (WorldToScreen(v[i], p1) && WorldToScreen(v[4 + i], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color2, 2.f);
	}
	//center of the box 
	bbcenter = (v[0] + v[6]) / 2;
}

void DrawBox(Matrix3x3 rotation, Vector3 bbmin, Vector3 bbmax, Vector3 position)
{
	Vector3 ax[6];
	ax[0] = Vector3{ rotation[0][0], rotation[0][1], rotation[0][2] }.Scale(bbmin.x);
	ax[1] = Vector3{ rotation[1][0], rotation[1][1], rotation[1][2] }.Scale(bbmin.y);
	ax[2] = Vector3{ rotation[2][0], rotation[2][1], rotation[2][2] }.Scale(bbmin.z);
	ax[3] = Vector3{ rotation[0][0], rotation[0][1], rotation[0][2] }.Scale(bbmax.x);
	ax[4] = Vector3{ rotation[1][0], rotation[1][1], rotation[1][2] }.Scale(bbmax.y);
	ax[5] = Vector3{ rotation[2][0], rotation[2][1], rotation[2][2] }.Scale(bbmax.z);

	Vector3 temp[6];
	temp[0] = position + ax[2];
	temp[1] = position + ax[5];
	temp[2] = temp[0] + ax[3];
	temp[3] = temp[1] + ax[3];
	temp[4] = temp[0] + ax[0];
	temp[5] = temp[1] + ax[0];

	Vector3 v[8];
	v[0] = temp[2] + ax[1];
	v[1] = temp[2] + ax[4];
	v[2] = temp[3] + ax[4];
	v[3] = temp[3] + ax[1];
	v[4] = temp[4] + ax[1];
	v[5] = temp[4] + ax[4];
	v[6] = temp[5] + ax[4];
	v[7] = temp[5] + ax[1];

	const auto draw = ImGui::GetBackgroundDrawList();

	ImColor color1 = ImColor(255, 0, 0);
	ImColor color2 = ImColor(255, 255, 255);

	Vector3 p1, p2;
	for (int i = 0; i < 4; i++)
	{

		if (WorldToScreen(v[i], p1) && WorldToScreen(v[(i + 1) & 3], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color1, 2.f);

		if (WorldToScreen(v[4 + i], p1) && WorldToScreen(v[4 + ((i + 1) & 3)], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color2, 2.f);

		if (WorldToScreen(v[i], p1) && WorldToScreen(v[4 + i], p2))
			draw->AddLine({ p1.x, p1.y }, { p2.x, p2.y }, color2, 2.f);
	}

}

void DrawOffscreenIndicator(Vector3 origin, float distance)
{
	ImRect screen_rect = { 0.0f, 0.0f, memory::scrsize.x, memory::scrsize.y };
	auto angle = atan2((memory::scrsize.y / 2) - origin.y, (memory::scrsize.x / 2) - origin.x);
	angle += origin.z > 0 ? M_PI : 0.0f;

	ImVec2 arrow_center{
		(memory::scrsize.x / 2) + cfg::off_radius * cosf(angle),
		(memory::scrsize.y / 2) + cfg::off_radius * sinf(angle)
	};
	std::array<ImVec2, 4> points{
		ImVec2(-22.0f, -8.6f),
			ImVec2(0.0f, 0.0f),
			ImVec2(-22.0f, 8.6f),
			ImVec2(-18.0f, 0.0f)
	};
	for (auto& point : points)
	{
		auto x = point.x * cfg::off_arrow_size;
		auto y = point.y * cfg::off_arrow_size;

		point.x = arrow_center.x + x * cosf(angle) - y * sinf(angle);
		point.y = arrow_center.y + x * sinf(angle) + y * cosf(angle);
	}
	auto draw = ImGui::GetBackgroundDrawList();
	float alpha = 1.0f;
	if (origin.z > 0)
	{
		constexpr float nearThreshold = 200.0f * 200.0f;
		ImVec2 screen_outer_diff =
		{
			origin.x < 0 ? abs(origin.x) : (origin.x > screen_rect.Max.x ? origin.x - screen_rect.Max.x : 0.0f),
			origin.y < 0 ? abs(origin.y) : (origin.y > screen_rect.Max.y ? origin.y - screen_rect.Max.y : 0.0f),
		};
		float distance = static_cast<float>(std::pow(screen_outer_diff.x, 2) + std::pow(screen_outer_diff.y, 2));
		alpha = origin.z < 0 ? 1.0f : (distance / nearThreshold);
	}
	auto arrowColor = ImColor((int)(cfg::off_color[0] * 255.f), (int)(cfg::off_color[1] * 255.f), (int)(cfg::off_color[2] * 255.f));
	arrowColor.Value.w = (std::min)(alpha, 1.0f);
	draw->AddTriangleFilled(points[0], points[1], points[3], arrowColor);
	draw->AddTriangleFilled(points[2], points[1], points[3], arrowColor);
	draw->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), 0.6f);
}

void DrawTextOnScreen(const ImVec2& position, const ImVec2& size, const std::string& text)
{
	auto draw = ImGui::GetBackgroundDrawList();


	draw->AddRectFilled({ position.x - (size.x * 0.5f) - 5, position.y + 5 },
		{ position.x + (size.x * 0.5f) + 5, position.y + 10 + (size.y * 0.5f) + 5 },
		ImColor(0, 0, 0, 100));

	draw->AddText({ position.x - (size.x * 0.5f), position.y + (size.y * 0.5f) },
		ImColor(255, 255, 255),
		text.c_str());
}

void DrawReloadBar(const ImVec2& position, int count, const ImVec2& size)// ALOT OF MAGIC NUMBERS HERE, I KNOW, BUT IT WORKS
{
	constexpr float stat = (10.f / 16);
	float progress = ((stat * count) * 0.1f);

	auto draw = ImGui::GetBackgroundDrawList();
	ImVec2 startPos{ position.x - (size.x * 0.5f) - 5, position.y + 10 + (size.y * 0.5f) + 5 };
	ImVec2 endPos{ position.x + (size.x * 0.5f) + 5, position.y + 10 + (size.y * 0.3f) + 10 };
	ImVec2 fillEndPos{ position.x - (size.x * 0.5f) + (progress * size.x) + 5, position.y + 10 + (size.y * 0.5f) + 10 };

	draw->AddRectFilled(startPos, endPos, ImColor(0, 0, 0, 150));
	ImColor fillColor = (progress == 1) ? ImColor(0, 255, 0, 200) : ImColor(255, 0, 0, 200);
	draw->AddRectFilled(startPos, fillEndPos, fillColor);
}

bool IsUnitValid(Unit* unit, Unit* local)
{
	if (!unit)
		return false;

	if (unit->TeamNum == local->TeamNum)
		return false;

	if (unit->Position.x == 0)
		return false;

	if (unit->UnitInfo->isPlane() && !cfg::show_planes)
		return false;

	return true;
}

//Causes crash

//bool is_alive(__int64 unit) //taken from ida game dump
//{
//	if (!unit) return false;
//	if (*reinterpret_cast<unsigned short*>(unit + 0xFF8) <= 1u)
//		return (*reinterpret_cast<unsigned int*>(unit + 0xFA8) & 0x20000) == 0 || reinterpret_cast<unsigned char(__fastcall*)(__int64)>(*reinterpret_cast<__int64*>(unit + 0x50) + 16)(unit + 0x50);
//	return false;
//}


void ESP()
{
	if (!cfg::esp_status)
		return;
	UnitList list = *reinterpret_cast<UnitList*>(memory::address::cGame + memory::offset::UnitList_3);
	if (!list.unitList)
		return;
	Player* localplayer = *reinterpret_cast<Player**>(memory::address::modulebase + memory::offset::LocalPlayer);
	bool isScoping = *reinterpret_cast<bool*>(memory::address::modulebase + memory::offset::IsScoping);
	char* curmap = *reinterpret_cast<char**>(memory::address::cGame + memory::offset::CurrentMap);

	if (localplayer->IsinHangar())
	{
		if (localplayer->ControlledUnit == NULL or localplayer->ControlledUnit->UnitInfo == NULL)
			return;
		auto position = localplayer->ControlledUnit->Position;
		const auto& rotation = localplayer->ControlledUnit->RotationMatrix;

		auto draw = ImGui::GetBackgroundDrawList();

		const Vector3& bbmin = localplayer->ControlledUnit->BBMin;
		const Vector3& bbmax = localplayer->ControlledUnit->BBMax;

		DrawBox(rotation, bbmin, bbmax, position);

		return;
	}

	const auto screen_center_x = memory::scrsize.x / 2;
	const auto screen_center_y = memory::scrsize.y / 2;

	std::uint16_t unitCount = *(std::uint16_t*)(memory::address::cGame + memory::offset::UnitCount_3);
	for (auto i = 0; i < unitCount; ++i)
	{
		Unit* unit = list.unitList->units[i];
		Unit* local = localplayer->ControlledUnit;
		Player* player = unit->PlayerInfo;

		if (!local)
			continue;

		if (IsUnitValid(unit, local))
		{
			const auto draw = ImGui::GetBackgroundDrawList();

			Vector3 position = unit->Position;
			float distance = local->Position.Distance(position);
			auto name = u8"";
			name = reinterpret_cast<char8_t*>(unit->UnitInfo->ShortName);
			auto text = std::format("{} - {}m", (char*)name, (int)distance, 2);
			ImVec2 size = ImGui::CalcTextSize(text.c_str());

			int count = (16 - (unit->VisualReload));
			Vector3 bbcenter{};

			if (player)
			{
				if (!player->IsAlive())
					continue;

				const auto& rotation = unit->RotationMatrix;
				const Vector3& bbmin = unit->BBMin;
				const Vector3& bbmax = unit->BBMax;

				DrawBox(rotation, bbmin, bbmax, position, unit->Invulnerable, bbcenter);

				Vector3 origin = { };
				if (WorldToScreen(position, origin))
				{
					if (origin.x < 0 || origin.x > memory::scrsize.x || origin.y < 0 || origin.y > memory::scrsize.y)
					{
						if (cfg::show_offscreen)
						{
							if (!isScoping)
							{
								DrawOffscreenIndicator(origin, distance);
							}
							continue;
						}

						continue;
					}

					DrawTextOnScreen(ImVec2(origin.x, origin.y), size, text);

					if (cfg::show_reload)
					{
						DrawReloadBar(ImVec2(origin.x, origin.y), count, size);
					}

				}
			}
			else
			{
				if (cfg::show_bots)
				{
					//if (strcmp(curmap, "levels/firing_range.bin") != 0) //If we wanna show bots only in firing range
					//	continue;

					if (!unit->IsAlive())
						continue;
					if (!localplayer->IsinHangar()) {
						if (unit->UnitInfo->isDummy())
							continue;
					}

					if (!cfg::show_planes)
					{
						if (unit->UnitInfo->isPlane())
							continue;
					}


					const auto& rotation = unit->RotationMatrix;
					const Vector3& bbmin = unit->BBMin;
					const Vector3& bbmax = unit->BBMax;
					text = std::format("BOT - {}m", (int)distance);
					size = ImGui::CalcTextSize(text.c_str());

					DrawBox(rotation, bbmin, bbmax, position, unit->Invulnerable, bbcenter);

					Vector3 origin = { };
					if (WorldToScreen(position, origin))
					{
						if (origin.x < 0 || origin.x > memory::scrsize.x || origin.y < 0 || origin.y > memory::scrsize.y)
						{
							if (cfg::show_offscreen)
							{

								if (!isScoping)
								{
									DrawOffscreenIndicator(origin, distance);
								}
							}
						}


						DrawTextOnScreen(ImVec2(origin.x, origin.y), size, text);

						if (cfg::show_reload && !unit->UnitInfo->isPlane())
						{
							DrawReloadBar(ImVec2(origin.x, origin.y), count, size);
						}

					}

				}
			}
		}
	}

	if (cfg::show_bombs)
	{
		ProjectileList bomblist{};
		std::uint16_t bombCount{};
		bomblist = *reinterpret_cast<ProjectileList*>(memory::address::modulebase + memory::offset::BombArray);
		bombCount = *reinterpret_cast<std::uint16_t*>(memory::address::modulebase + memory::offset::BombArray + 0x10);

		for (auto i = 0; i < bombCount; ++i)
		{
			const auto unit = bomblist.projectileList->projectiles[i];
			if (!unit)
				continue;
			auto draw = ImGui::GetBackgroundDrawList();
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << unit->getExpodeTime();
			std::string time_str = oss.str();
			const char* time = time_str.c_str();

			Vector3 origin = { };
			if (WorldToScreen(unit->Position, origin))
			{
				if (origin.x < 0 || origin.x > memory::scrsize.x || origin.y < 0 || origin.y > memory::scrsize.y)
				{
					continue;
				}
				if (cfg::Bout_type == 0) {
					ImGui::PushFont(menu::icons);
					const char* name = "A";
					const auto size = ImGui::CalcTextSize(name);
					const auto size2 = ImGui::CalcTextSize(time);

					draw->AddText(ImVec2(origin.x - size.x / 2, origin.y - size.y / 2), IM_COL32(255, 255, 255, 255), name);
					ImGui::PopFont();
					ImGui::PushFont(menu::def_main);
					draw->AddText(ImVec2(origin.x - size2.x / 4, origin.y + size.y), IM_COL32(255, 255, 255, 255), time);
					ImGui::PopFont();
				}
				else {
					ImGui::PushFont(menu::def_main);
					const char* name = "BOMB";
					const auto size = ImGui::CalcTextSize(name);
					const auto size2 = ImGui::CalcTextSize(time);

					draw->AddText(ImVec2(origin.x - size.x / 2, origin.y - size.y / 2), IM_COL32(255, 255, 255, 255), name);
					draw->AddText(ImVec2(origin.x - size2.x / 2, origin.y - size.y / 2 + size.y), IM_COL32(255, 255, 255, 255), time);
					ImGui::PopFont();
				}
			}
		}
	}
	if (cfg::show_rockets)
	{
		ProjectileList rocketList{};
		std::uint16_t rocketCount{};
		rocketList = *reinterpret_cast<ProjectileList*>(memory::address::modulebase + memory::offset::RocketArray);
		rocketCount = *reinterpret_cast<std::uint16_t*>(memory::address::modulebase + memory::offset::RocketArray + 0x10);

		for (auto i = 0; i < rocketCount; ++i)
		{
			const auto unit = rocketList.projectileList->projectiles[i];
			if (!unit)
				continue;
			auto draw = ImGui::GetBackgroundDrawList();
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(1) << localplayer->ControlledUnit->Position.Distance(unit->Position);
			std::string dist_str = oss.str();
			const char* text2 = dist_str.c_str();
			const auto size2 = ImGui::CalcTextSize(text2);
			Vector3 origin = { };
			if (WorldToScreen(unit->Position, origin))
			{
				if (origin.x < 0 || origin.x > memory::scrsize.x || origin.y < 0 || origin.y > memory::scrsize.y)
				{
					continue;
				}
				else
				{
					if (cfg::Mout_type == 0) {
						ImGui::PushFont(menu::icons);
						const char* text = "B";
						const auto size = ImGui::CalcTextSize(text);
						draw->AddText(ImVec2(origin.x - size.x / 2, origin.y - size.y / 2), IM_COL32(255, 255, 255, 255), text);
						ImGui::PopFont();
						ImGui::PushFont(menu::def_main);
						draw->AddText(ImVec2(origin.x - size2.x / 2, origin.y - size.y / 2 + size.y), IM_COL32(255, 255, 255, 255), text2);
						ImGui::PopFont();
					}
					else {
						ImGui::PushFont(menu::def_main);
						const char* text = "x";
						const auto size = ImGui::CalcTextSize(text);
						draw->AddText(ImVec2(origin.x - size.x / 2, origin.y - size.y / 2), IM_COL32(255, 255, 255, 255), text);
						draw->AddText(ImVec2(origin.x - size2.x / 2, origin.y - size.y / 2 + size.y), IM_COL32(255, 255, 255, 255), text2);
						ImGui::PopFont();
					}

				}
			}
		}
	}
}


void drawUnitWindow(UnitList list, std::uint16_t Count, Player* localplayer) {
	for (auto i = 0; i < Count; ++i)
	{
		const auto unit = list.unitList->units[i];
		if (!unit)
			continue;
		auto local = localplayer->ControlledUnit;
		if (!local)
			continue;
		if (local->Position.x == 0)
			continue;
		const auto id = reinterpret_cast<std::intptr_t>(unit) + i;
		if (ImGui::TreeNode(reinterpret_cast<void*>(id), "%s Unit: %s", unit->PlayerInfo == nullptr ? "Bot" : "Player", unit->UnitInfo->unitName)) {
			ImGui::Text("Bot: %s", unit->PlayerInfo == nullptr ? "true" : "false");
			ImGui::Text("Is plane: %s", unit->UnitInfo->isPlane() ? "true" : "false");
			ImGui::Text("Is dummy: %s", unit->UnitInfo->isDummy() ? "true" : "false");
			ImGui::Text("Position: %f %f %f", unit->Position.x, unit->Position.y, unit->Position.z);
			ImGui::Text("Distance: %f", unit->Position.Distance(localplayer->ControlledUnit->Position));
			ImGui::Text("Invul? %s", unit->Invulnerable ? "true" : "false");
			ImGui::Text("Address: %p", unit);
			if (ImGui::IsItemClicked()) {
				std::stringstream stream{};
				stream << std::hex << reinterpret_cast<std::uintptr_t>(unit);
				ImGui::SetClipboardText(stream.str().c_str());
			}

			ImGui::Text("Unitstate: %d", unit->UnitState);
			if (ImGui::TreeNode(reinterpret_cast<void*>(id + 1), "Unit info")) {
				ImGui::Text("Unit name: %s", unit->UnitInfo->unitName);
				ImGui::Text("Model name: %s", unit->UnitInfo->modelName);
				ImGui::Text("Unit type: %s", unit->UnitInfo->unitType);
				ImGui::Text("Full name: %s", unit->UnitInfo->FullName);
				ImGui::Text("Short name: %s", unit->UnitInfo->ShortName);
				ImGui::Text("Zoom: %f", unit->UnitInfo->ZoomMulti);
				ImGui::Text("Alt zoom: %f", unit->UnitInfo->AlternateMulti);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}

void drawProjectileWindow(ProjectileList list, std::uint16_t Count) {
	for (auto i = 0; i < Count; ++i)
	{
		const auto unit = list.projectileList->projectiles[i];
		if (!unit)
			continue;
		const auto id = reinterpret_cast<std::intptr_t>(unit) + i;
		if (ImGui::TreeNode(reinterpret_cast<void*>(id), "Unit %s", unit->Name)) {
			ImGui::Text("Position: X: %f, Y: %f, Z: %f", unit->Position.x, unit->Position.y, unit->Position.z);
			ImGui::Text("Owner: %s", unit->OwnerUnit->UnitInfo->unitName);
			ImGui::Text("Delay: %f", unit->BombDelay);
			ImGui::Text("Explotion time %f", unit->getExpodeTime());
			ImGui::Text("Address: %p", unit);
			if (ImGui::IsItemClicked()) {
				std::stringstream stream{};
				stream << std::hex << reinterpret_cast<std::uintptr_t>(unit);
				ImGui::SetClipboardText(stream.str().c_str());
			}

			ImGui::TreePop();
		}
	}
}


void debug() {
	ImGui::SetNextWindowSize({ 400, 400 });
	ImGui::Begin("Unit List");
	static const char* items[] = { "Unit List 1", "Unit List 2", "Unit List 3", "BombList", "RocketList" };
	static int currentItem = 0;

	UnitList list{};
	ProjectileList plist{};
	std::uint16_t Count{};

	ImGui::Combo("Unit List", &currentItem, items, IM_ARRAYSIZE(items));

	Player* localplayer = *reinterpret_cast<Player**>(memory::address::modulebase + memory::offset::LocalPlayer);
	if (!localplayer)
		return;
	switch (currentItem)
	{
	case 0:
		list = *reinterpret_cast<UnitList*>(memory::address::cGame + memory::offset::UnitList_1);
		Count = *reinterpret_cast<std::uint16_t*>(memory::address::cGame + memory::offset::UnitCount_1);
		drawUnitWindow(list, Count, localplayer);
		break;
	case 1:
		list = *reinterpret_cast<UnitList*>(memory::address::cGame + memory::offset::UnitList_2);
		Count = *reinterpret_cast<std::uint16_t*>(memory::address::cGame + memory::offset::UnitCount_2);
		drawUnitWindow(list, Count, localplayer);
		break;
	case 2:
		list = *reinterpret_cast<UnitList*>(memory::address::cGame + memory::offset::UnitList_3);
		Count = *reinterpret_cast<std::uint16_t*>(memory::address::cGame + memory::offset::UnitCount_3);
		drawUnitWindow(list, Count, localplayer);
		break;
	case 3:
		plist = *reinterpret_cast<ProjectileList*>(memory::address::modulebase + memory::offset::BombArray);
		Count = *reinterpret_cast<std::uint16_t*>(memory::address::modulebase + memory::offset::BombArray + 0x10);
		drawProjectileWindow(plist, Count);
		break;
	case 4:
		plist = *reinterpret_cast<ProjectileList*>(memory::address::modulebase + memory::offset::RocketArray);
		Count = *reinterpret_cast<std::uint16_t*>(memory::address::modulebase + memory::offset::RocketArray + 0x10);
		drawProjectileWindow(plist, Count);
		break;
	default:
		break;
	}

	ImGui::End();

}