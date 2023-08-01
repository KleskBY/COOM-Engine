#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"

inline ImFont* font_default;
inline ImFont* font_menu;
inline ImFont* font_hud;

inline int ButtonWidth = 200;
inline int ButtonHeight = 50;

inline float FontScale = 0.5f;
inline float FontScaleBig = 1.f;

inline ImVec2 LastWindowSize;

void Rescale();
void CenterWindow(int w, int h, bool center = false);
void SetupFonts();


//themes
void Style();
void StyleGrey();
void StyleValve();
void StyleDeepDark();
