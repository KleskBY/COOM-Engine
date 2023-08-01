//
// Main.cpp
//

#include "pch.h"
#include "main.h"
#include "Game.h"
#include "WndProc.h"

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
    std::unique_ptr<Game> g_game;
}



// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    AllocConsole();
    FILE* f = { nullptr };
    freopen_s(&f, "CONOUT$", "w", stdout);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!XMVerifyCPUSupport()) return 1;

    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr)) return 1;

    g_game = std::make_unique<Game>();

    // Register class and create window
    {
        // Register class
        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
        wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszClassName = WindowClass.c_str();
        wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
        if (!RegisterClassExW(&wcex)) return 1;

        // Create window
        int w, h;
        g_game->GetDefaultSize(w, h);
        RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        hwnd = CreateWindowExW(0, WindowClass.c_str(), WindowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
            CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
        if (!hwnd) return 1;
        // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"GameEngineWindowClass", g_szAppName, WS_POPUP, to default to fullscreen.


        ShowWindow(hwnd, nCmdShow);
        // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));
        GetClientRect(hwnd, &rc);
        g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
    }

    // Main message loop
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_game->Tick();
        }
    }

    g_game.reset();
    CoUninitialize();
    return static_cast<int>(msg.wParam);
}
