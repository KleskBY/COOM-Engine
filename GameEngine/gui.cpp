#include "pch.h"
#include "gui.h"
#include "main.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace gui
{
    bool init = false;
    bool open = true;
	void RenderGUI(ID3D11Device* device, ID3D11DeviceContext* context)
	{
        if (!init)
        {
            init = true;

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.DisplaySize.x = 100;
            io.DisplaySize.y = 100;
            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX11_Init(device, context);
        }
        else
        {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            ImGuiIO& io = ImGui::GetIO();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
            ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
            if (ImGui::Begin("##gui", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs))
            {
                ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
                ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);





                ImGui::Text("COOM ENGINE v0.1");
                ImGui::Text(__DATE__);



                ImGui::GetWindowDrawList()->PushClipRectFullScreen();
                ImGui::End();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2);
            }

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
	}
}