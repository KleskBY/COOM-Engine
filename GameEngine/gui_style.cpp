#include "pch.h"
#include "gui_style.h"

#include "main.h"


float proportionalValue(float argument) 
{
	if (argument == 0.0f) return 1.0f;

	const float minArg = 0.1f;
	const float maxArg = 2560.0f; // Max res (QHD or ultrwide)

	const float minValue = 0.1f;
	const float maxValue = 2.0f;

	float proportion = (argument - minArg) / (maxArg - minArg);
	float result = minValue + proportion * (maxValue - minValue);

	if (argument >= maxArg) return maxValue;
	else if (argument <= minArg) return minValue;
	else return result;
}

void Rescale()
{
	auto& io = ImGui::GetIO();
	if (io.DisplaySize.x == 0) return;
	if (io.DisplaySize.x != LastWindowSize.x)
	{
		LastWindowSize = io.DisplaySize;

		ButtonWidth = io.DisplaySize.x / 4;
		ButtonHeight = ButtonWidth / 4;
		float s = proportionalValue(io.DisplaySize.x);
		font_default->Scale = s;
		font_menu->Scale = s;
		font_hud->Scale = s;
	}
}
void CenterWindow(int w, int h, bool center)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetWindowSize(ImVec2(w, h), ImGuiCond_Once);
	if (center) ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2 - w / 2, io.DisplaySize.y / 2 - h / 2), ImGuiCond_Once);
	else ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2 - w / 3, io.DisplaySize.y / 2 - h / 2), ImGuiCond_Once);
}
void SetupFonts()
{
	auto io = ImGui::GetIO();
	font_default = io.Fonts->AddFontFromFileTTF((PATH_FONTS + "Boxy-Bold.ttf").c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	font_menu = io.Fonts->AddFontFromFileTTF((PATH_FONTS + "Boxy-Bold.ttf").c_str(), 24.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	font_hud = io.Fonts->AddFontFromFileTTF((PATH_FONTS + "Boxy-Bold.ttf").c_str(), 48.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
}

void Style()
{
	ImGuiStyle* style = &ImGui::GetStyle(); // Setup style
	style->Colors[ImGuiCol_Text] = ImColor(220, 220, 220, 220);
	style->Colors[ImGuiCol_TextDisabled] = ImColor(53, 53, 53, 255);
	style->Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_ChildBg] = ImColor(25, 25, 25, 225);
	style->Colors[ImGuiCol_PopupBg] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_Border] = ImColor(35, 35, 35, 150);
	style->Colors[ImGuiCol_FrameBg] = ImColor(31, 31, 31, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(230, 0, 0, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(255, 0, 0, 175);
	style->Colors[ImGuiCol_TitleBg] = ImColor(31, 31, 31, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(22, 22, 22, 255);
	style->Colors[ImGuiCol_MenuBarBg] = ImColor(25, 25, 25, 255);
	style->Colors[ImGuiCol_ScrollbarBg] = ImColor(20, 20, 20, 255);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(242, 0, 0, 232);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(241, 0, 0, 174);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 0, 0, 133);
	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 0, 0, 253);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(246, 0, 0, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 0, 0, 175);
	//style->Colors[ImGuiCol_Button] = ImColor(126, 112, 85, 245); //tarkov style
	//style->Colors[ImGuiCol_ButtonHovered] = ImColor(154, 136, 102, 140);
	style->Colors[ImGuiCol_Button] = ImColor(245, 0, 0, 245);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(200, 0, 0, 150);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 168);
	style->Colors[ImGuiCol_Header] = ImColor(255, 0, 0, 175);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(244, 0, 0, 223);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(255, 0, 0, 255);
	style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 78);

	style->FrameBorderSize = 0;
	style->FrameRounding = 4;
	style->ItemSpacing = ImVec2(5, 5);
	style->GrabRounding = 4;
	style->WindowRounding = 0;
	style->ScrollbarRounding = 4;
	style->ScrollbarSize = ButtonWidth / 20;
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
}

void StyleGrey()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	/// 0 = FLAT APPEARENCE
	/// 1 = MORE "3D" LOOK
	int is3D = 1;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding = ImVec2(6, 4);
	style.ItemSpacing = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = is3D;

	style.WindowRounding = 3;
	style.ChildRounding = 3;
	style.FrameRounding = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding = 3;
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;

#ifdef IMGUI_HAS_DOCK 
	style.TabBorderSize = is3D;
	style.TabRounding = 3;

	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
#endif
}

void StyleValve()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);// ImVec4(0.19f, 0.23f, 0.18f, 0.00f); // grip invis
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
	colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	//colors[ImGuiCol_DockingPreview] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 1.0f;
	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
}

void StyleDeepDark()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	//colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	//colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.TabBorderSize = 1;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 3;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}

static void HelpMarker(const char* desc)
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
static void CustomGlow(ImVec2 pos1, ImVec2 size, ImColor centColor, ImColor endColor)
{
	//ImGui::GetWindowDrawList()->AddRectFilledMultiColor(pos1, ImVec2(pos1.x + pos2.x/2, pos2.y/2), endColor, colorCenter, endColor, colorCenter); //TopLeftRect
	//ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x + pos2.x / 2, pos1.y + pos2.y / 2), pos2, endColor, colorCenter, endColor, colorCenter); //BotLeftRect
	int smallWidth = size.x / 4;
	int w = smallWidth * 2;
	int h = size.y / 2;
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x - smallWidth, pos1.y + h), endColor, endColor, endColor, centColor);//TopLeft
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x, pos1.y + h), ImVec2(pos1.x - smallWidth, pos1.y + h * 2), centColor, endColor, endColor, endColor);//BotLeft
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(pos1, ImVec2(pos1.x + w, pos1.y + h), endColor, endColor, centColor, centColor); //MIdTop
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x, pos1.y + h), ImVec2(pos1.x + w, pos1.y + h * 2), centColor, centColor, endColor, endColor);//MidBot
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x + w, pos1.y), ImVec2(pos1.x + w + smallWidth, pos1.y + h), endColor, endColor, endColor, centColor);//RightTop
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(pos1.x + w, pos1.y + h), ImVec2(pos1.x + w + smallWidth, pos1.y + h * 2), centColor, endColor, endColor, endColor);//RightBot

	//ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(0, 200), ImVec2(400, 400), ImColor(0.9f, 0.75f, 0.41f, 0.8f), ImColor(0.9f, 0.75f, 0.41f, 0.8f), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0));

}

static const char* KeysList[] =
{
	"NONE", "MOUSE1", "MOUSE2", "CANCEL", "MOUSE 3", "MOUSE 4", "MOUSE 5", "None", "BACKSPACE",
	"TAB", "None", "None", "CLEAR", "ENTER", "None",  "None",  "SHIFT",  "CTRL",  "ALT",  "PAUSE",
	"CAPS LOCK",  "KANA",  "HANGUEL",  "HANGUL",  "JUNIA",  "FINAL",  "HANJA",
	"ESC",  "CONVERT",  "NONCONVERT",  "ACCEPT",  "MODE",  "SPACE",  "PAGE UP",  "PAGE DOWN",
	"END", "HOME", "LEFT ARROW", "UP ARROW", "RIGHT ARROW", "DOWN ARROW", "SELECT", "PRINT", "EXECUTE",
	"PRINT SCREEN", "INSERT", "DELETE", "HELP", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "a", "b", "c", "d", "e", "f", "g", "h",
	"i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
	"WIN", "WIN", "APPS", "NONE", "SLEEP", "KP_INS", "KP_END", "KP_DOWNARROW", "KP_PGDN", "KP_LEFTARROW",
	"KP_5", "KP_RIGHTARROW", "KP_HOME", "KP_UPARROW", "KP_PGUP", "kp_multiply", "kp_plus", "kp_slash", "kp_minus",
	"kp_del", "kp_slash", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
	"NUM LOCK", "SCROLL LOCK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
	"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
	"LSHIFT", "RSHIFT", "LCTRL", "RCTRL", "LALT", "RALT", "BACK", "NONE", "MWHEELUP", "MWHEELDOWN"
};
static int KeyNameToKeyNumber(std::string keyName)
{
	for (int i = 0; i < 170; i++)
	{
		if (KeysList[i])
		{
			if (keyName == std::string(KeysList[i])) return i;

		}
	}
	return 0;
}


inline bool SetKey = false;
inline bool once2 = false;
inline bool mwheelup = false;
inline double lastWheel;
static int KeySelector(int* key, std::string label)
{

	std::string KeyLabel = KeysList[*key];
	static std::string SelectedKeyLabel = "";
	int ret = 0;

	if (SelectedKeyLabel == label) KeyLabel = "<PRESS>";
	if (ImGui::Button((KeyLabel + "##" + label + "Label").c_str(), ImVec2(ButtonWidth, ButtonHeight * 0.4)))
	{
		SelectedKeyLabel = label;
		SetKey = true;
	}
	if (SetKey && SelectedKeyLabel == label)
	{

		for (int i = 0; i < sizeof(KeysList); i++)
		{
			if (lastWheel + 0.1 > ImGui::GetTime())
			{
				if (mwheelup)
				{
					*key = 168;
				}
				else
				{
					*key = 169;
				}
				KeyLabel = KeysList[*key];
				SetKey = false;
				SelectedKeyLabel = "";
				ret = *key;
				break;
			}
			if (GetAsyncKeyState(i) & 0x8000)
			{
				*key = i;
				KeyLabel = KeysList[i];
				SetKey = false;
				SelectedKeyLabel = "";
				ret = i;
				break;
			}
		}
	}
	ImGui::SameLine();  ImGui::Text(label.c_str());
	return ret;
}
