//
// Game.cpp
//
#include "pch.h"
#include "Game.h"
#include "main.h"
#include "render.h"
#include "postprocess.h"

#include "hkOnDevice.h"
#include "InputManager.h"
#include "GameUpdate.h"
#include "LevelBuilder.h"
#include "Viewmodel.h"


using namespace DirectX;
using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_pitch = 0;
    m_yaw = 0;

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
    //m_timer.SetFixedTimeStep(true);
    //m_timer.SetTargetElapsedSeconds(1.0 / 120);

    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });
    Render();
}

void Game::Update(DX::StepTimer const& timer)
{   
    GameUpdate(timer);
    // TODO: Add your game logic here.
}
#pragma endregion


#pragma region Frame Render

#include "gui.h"
// Draws the scene.
void Game::Render()
{
    if (m_timer.GetFrameCount() == 0) return;     // Don't try to render anything before the first Update.
    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    Renderer(m_deviceResources->GetD3DDevice(), context, m_deviceResources->GetSwapChain());
    //postprocess::Run(m_deviceResources->GetD3DDevice(), context, m_deviceResources->GetSwapChain()); //blur
    gui::RenderGUI(m_deviceResources->GetD3DDevice(), context); //imgui


    m_deviceResources->PIXEndEvent();
    m_deviceResources->Present(); // Show the new frame.
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
    WindowWidth = r.right;
    WindowHeight = r.bottom;
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height)) return;
    CreateWindowSizeDependentResources();
    WindowWidth = width;
    WindowHeight = height;
    // TODO: Game window is being resized.
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = WindowWidth;
    height = WindowHeight;
}
#pragma endregion

#pragma region Direct3D Resources

#include "dx_tools.h"
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    m_states = std::make_unique<CommonStates>(device);
    m_fxFactory = std::make_unique<EffectFactory>(device);
    m_world = Matrix::Identity;
    m_spriteBatch = std::make_unique<SpriteBatch>(context);
    GenerateTexture(device, m_texture.ReleaseAndGetAddressOf()); //Temp texture for testing

    //Main effect for drawing level
    m_effect = std::make_unique<BasicEffect>(device);
    m_effect->SetWorld(m_world);
    m_effect->SetView(m_view);
    m_effect->SetProjection(m_proj);
    m_effect->SetTexture(m_texture.Get());
    m_effect->SetTextureEnabled(true);

    m_effect->EnableDefaultLighting();          //ugly specular glowing
    m_effect->SetSpecularColor(Colors::Black);  //disable specular glow
    //m_effect->SetAmbientLightColor(Colors::Gray);
    //m_effect->SetDiffuseColor(Colors::Gray);

    m_effect->SetFogEnabled(true);
    m_effect->SetFogColor(Colors::Gray);
    m_effect->SetFogStart(0.f);
    m_effect->SetFogEnd(30.f);
    DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexType>(device, m_effect.get(), &m_inputLayout));

    //Basic drawing fore debug and shit
    primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
    basicEffect = std::make_unique<BasicEffect>(device);
    basicEffect->SetVertexColorEnabled(true);
    void const* shaderByteCode;
    size_t byteCodeLength;
    basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
    HRESULT hr = device->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, &inputLayout);
    if (FAILED(hr)) { printf("Unable to create device input layout; result = %d\n", hr); }

    postprocess::Init(device, context, m_deviceResources->GetSwapChain());

    // TODO: Initialize device dependent objects here (independent of window size).
   
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    m_proj = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(FOV), float(size.right) / float(size.bottom), 0.01f, RenderDistance);
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    hkOnDeviceLost();
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    hkOnDeviceRestored(this);
}
#pragma endregion
