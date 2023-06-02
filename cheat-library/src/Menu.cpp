#include "../header/Menu.h"
#include "../header/Memory.h"
#include "../header/Config.h"

bool menu::agree = false;
bool menu::open = true;
bool menu::tab_esp = false;
bool menu::tab_misc = false;
bool menu::tab_support = false;
bool menu::def_tab = true;
bool menu::def_choice = false;
ImVec2 menu::sizeScr = ImVec2(0, 0);
ImFont* menu::def_main = nullptr;
ImFont* menu::med_main = nullptr;
ImFont* menu::big_main = nullptr;
ImFont* menu::icons = nullptr;

void menu::SetupImGuiStyle()
{
	ImGui::GetStyle().FrameRounding = 6.f;
	ImGui::GetStyle().WindowBorderSize = 0.f;
	ImGui::GetStyle().ChildBorderSize = 0.f;
	ImGui::GetStyle().WindowTitleAlign.x = 0.5f;
	ImGui::GetStyle().WindowTitleAlign.y = 0.4f;
	ImGui::GetStyle().TabRounding = 0.f;
	ImGui::GetStyle().WindowPadding.x = 0.f;


	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.07f, 0.11f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.07f, 0.11f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.07f, 0.11f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.14f, 0.22f, 0.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.14f, 0.22f, 0.72f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.14f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.31f, 0.15f, 0.31f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.31f, 0.15f, 0.31f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.31f, 0.15f, 0.31f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.49f, 0.10f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.49f, 0.10f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.79f, 0.15f, 0.81f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.49f, 0.10f, 0.51f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.79f, 0.15f, 0.81f, 0.79f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.79f, 0.15f, 0.81f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.02f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.49f, 0.10f, 0.51f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.79f, 0.15f, 0.81f, 0.78f);
	colors[ImGuiCol_TabActive] = ImVec4(0.79f, 0.15f, 0.81f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

void menu::showWarningwindow()
{
	auto text1 = "SOFTWARE INI DIDISTRIBUSIKAN SECARA GRATIS.";
	auto text2 = "JIKA ANDA MEMBAYAR UNTUK PERANGKAT LUNAK INI - ANDA TELAH DITIPU.";
	auto text3 = "Ubah handle klik di bagian tab \"Lainnya\" (Blokir masukan pengguna)";
	ImGui::SetNextWindowSize({ memory::scrsize.x, memory::scrsize.y });
	ImGui::SetNextWindowPos({ 0, 0 });
	auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("PERINGATAN", nullptr, flags);
	ImGui::PushFont(big_main);
	auto textWidth1 = ImGui::CalcTextSize(text1).x;
	ImGui::SetCursorPos(ImVec2((memory::scrsize.x - textWidth1) * 0.5f, 100));
	ImGui::Text(text1);
	auto textWidth2 = ImGui::CalcTextSize(text2).x;
	ImGui::SetCursorPosX((memory::scrsize.x - textWidth2) * 0.5f);
	ImGui::Text(text2);
	ImGui::SetCursorPosX((memory::scrsize.x - 225) * 0.5f);
	auto textWidth3 = ImGui::CalcTextSize(text3).x;
	ImGui::SetCursorPosX((memory::scrsize.x - textWidth3) * 0.5f);
	ImGui::Text(text3);
	ImGui::SetCursorPosX((memory::scrsize.x - 225) * 0.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::Checkbox("SAYA MENGERTI", &agree);
	ImGui::PopStyleVar();
	ImGui::PopFont();
	ImGui::End();
}

void menu::showMenu() {

	ImGui::PushFont(med_main);
	ImGui::SetNextWindowSize({ 550,350 });
	ImGui::SetNextWindowPos({ (memory::scrsize.x - 550) * 0.5f, (memory::scrsize.y - 350) * 0.5f }, ImGuiCond_FirstUseEver);
	ImGui::Begin("WarMil", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::BeginTabBar("main");
	ImGui::SetNextItemWidth(180.f);
	if (ImGui::BeginTabItem("\t\t\t\tESP", &tab_esp, ImGuiTabItemFlags_NoCloseButton))
	{

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,5 });
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Aktifkan", &cfg::esp_status);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan bar reload", &cfg::show_reload);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan bot", &cfg::show_bots);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan pesawat", &cfg::show_planes);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan bom", &cfg::show_bombs);
		if (cfg::show_bombs) {
			ImGui::SameLine();
			ImGui::RadioButton("Icon##0", &cfg::Bout_type, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Text##0", &cfg::Bout_type, 1);
		}
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan roket", &cfg::show_rockets);
		if (cfg::show_rockets) {
			ImGui::SameLine();
			ImGui::RadioButton("Icon##1", &cfg::Mout_type, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Text##1", &cfg::Mout_type, 1);
		}
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Hilangkan asap", &cfg::remove_smokes);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Tampilkan panah di layar", &cfg::show_offscreen);
		if (cfg::show_offscreen)
		{
			ImGui::SameLine();
			ImGui::ColorEdit3("Warna panah", cfg::off_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			ImGui::Indent();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			ImGui::SliderFloat("Ukuran panah", &cfg::off_arrow_size, 0.0f, 3.f);
			ImGui::SliderFloat("Radius", &cfg::off_radius, 0.0f, 1000.0f);
			ImGui::PopStyleVar();
		}
		ImGui::PopStyleVar(2);
		ImGui::EndTabItem();
	}

	ImGui::SetNextItemWidth(180.f);
	if (ImGui::BeginTabItem("\t\t\t\Misc", &tab_misc, ImGuiTabItemFlags_NoCloseButton))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,5 });
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Modifikasi zoom", &cfg::zoom_mod);
		ImGui::SetCursorPosX(5.f);
		ImGui::Text("Zoom multiplier");
		ImGui::SetCursorPosX(5.f);
		ImGui::SliderFloat("##zoom", &cfg::zoom_mult, 1.0f, 100.0f);
		ImGui::SetCursorPosX(5.f);
		ImGui::Text("Alternatif multiplier");
		ImGui::SetCursorPosX(5.f);
		ImGui::SliderFloat("##alt", &cfg::alt_mult, 1.0f, 100.0f);
		ImGui::SetCursorPosX(5.f);
		ImGui::Text("Multiplier zoom bayangan");
		ImGui::SetCursorPosX(5.f);
		ImGui::SliderFloat("##shadow", &cfg::shadow_mult, 20.0f, 250.0f);
		ImGui::SetCursorPosX(5.f);
		ImGui::Checkbox("Ubah HUD", &cfg::change_hud);
		/*if (change_hud)
		{
			ImGui::SetCursorPosX(5.f);
			ImGui::Checkbox("Enable bomb crosshair", &force_bombsight);
			ImGui::SetCursorPosX(5.f);
			ImGui::Checkbox("Enable Air-To_air indicator", &force_air_to_air);
			ImGui::SetCursorPosX(5.f);
			ImGui::Checkbox("Enable enemy outline (when hovered)", &force_outline);
			ImGui::SetCursorPosX(5.f);
			ImGui::Checkbox("Show distance in scope", &force_distance);
			ImGui::SetCursorPosX(5.f);
			ImGui::Checkbox("Show penetrarion indicator", &force_crosshair);
		}
		*/

		ImGui::PopStyleVar(2);
		ImGui::EndTabItem();

	}
	ImGui::SetNextItemWidth(180.f);
	if (ImGui::BeginTabItem("\t\t\tLainnya", &tab_support, ImGuiTabItemFlags_NoCloseButton))
	{
		ImGui::SetCursorPosX(5.f);
		ImGui::Text("Versi saat ini: 1.8 (Terbaru)");
		ImGui::SetCursorPosX(5.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::Checkbox("Blokir input pengguna saat menu dibuka", &cfg::block_input);
		ImGui::PopStyleVar();
		ImGui::SetCursorPosX(5.f);
		ImGui::Text("Dukung penulis:");
		std::vector<std::pair<std::string, std::string>> support{
			{"Ko-Fi", "https:\/\/ko-fi.com\/soevielofficial"},
			{"Saweria","https:\/\/saweria.co\/soevielofficial" }
		};
		int i = 0;
		ImGui::Indent();
		for (auto& [key, value] : support) {
			ImGui::Text("%s", key.c_str());
			ImGui::SameLine();
			ImGui::PushID(i++);
			if (ImGui::Button("Copy")) {
				ImGui::SetClipboardText(value.c_str());
			}
			ImGui::PopID();
		}

		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	if (!tab_esp && !tab_misc && !tab_support)
	{
		if (def_tab)
		{
			auto text1 = "This tool made by soevielofficial";
			auto windowWidth = ImGui::GetWindowSize().x;
			auto textWidth = ImGui::CalcTextSize(text1).x;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::Text(text1);

			ImGui::SetCursorPos({ (windowWidth - 100) * 0.5f ,150 });
			ImGui::Button("Start", { 100,30 });

			if (ImGui::IsItemClicked())
			{
				tab_esp = true;
				tab_support = true;
				tab_misc = true;
				def_tab = false;
			}
		}
	}
	ImGui::PopFont();

	ImGui::End();
}