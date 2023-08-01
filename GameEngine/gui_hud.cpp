#include "pch.h"
#include "gui_hud.h"
#include "gui_style.h"
#include "imgui/imgui.h"
#include "PlayerController.h"

void DrawHud()
{
	ImGui::PushFont(font_hud);

	auto io = ImGui::GetIO();
	ImGui::SetCursorPos(ImVec2(10, io.DisplaySize.y - ImGui::GetFontSize()));

	if(player::PlayerHealth > 0) ImGui::Text(("+" + std::to_string(player::PlayerHealth)).c_str());
	ImGui::PopFont();
}