//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include "PerlinNoise.hpp"
#include "Engine/Shader.h"
#include "Engine/Buffers.h"
#include "Engine/VertexLayout.h"
#include "Engine/Texture.h"
#include "Engine/DefaultResources.h"
#include "Gameplay/World.h"
#include "Gameplay/Player.h"
#include "Gameplay/Utils.h"
#include "Engine/Light.h"
#include "Gameplay/Skybox.h"
#include "string"
#include "iostream"


extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

// Global stuff
DefaultResources gpuResources;
Shader basicShader(L"Basic");
Shader blockShader(L"Block");
World world;
Player player(&world, Vector3(0, 4, 10));
OrthographicCamera hudCamera(1280, 720);

/// <summary>
/// Creates a new game
/// </summary>
Game::Game() noexcept(false) {
	m_deviceResources = std::make_unique<DeviceResources>(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, DXGI_FORMAT_D32_FLOAT, 2);
	m_deviceResources->RegisterDeviceNotify(this);
}

/// <summary>
/// Destroys the game
/// </summary>
Game::~Game() {
	g_inputLayouts.clear();
}

/// <summary>
/// Initialize the game
/// </summary>
/// <param name="window">The game'window</param>
/// <param name="width">The window's width</param>
/// <param name="height">The window's height</param>
/// <param name="mode">The game mode</param>
void Game::Initialize(HWND window, int width, int height, GameMode mode) {
	// Create input devices
	m_gamePad = std::make_unique<GamePad>();
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);
	m_mouse->SetMode(Mouse::MODE_ABSOLUTE);

	m_mode = mode;

	// Initialize the Direct3D resources
	m_deviceResources->SetWindow(window, width, height);
	m_deviceResources->CreateDeviceResources();
	m_deviceResources->CreateWindowSizeDependentResources();

	basicShader.Create(m_deviceResources.get());
	blockShader.Create(m_deviceResources.get());
	GenerateInputLayout<VertexLayout_PositionColor>(m_deviceResources.get(), &basicShader);
	//GenerateInputLayout<VertexLayout_PositionNormalUV>(m_deviceResources.get(), &blockShader);
	GenerateInputLayout<VertexLayout_PositionNormalUVInstanced>(m_deviceResources.get(), &blockShader);

	// Initialize GPU resources
	gpuResources.Create(m_deviceResources.get());

	// Initialize player & cameras
	player.GenerateGPUResources(m_deviceResources.get(),mode);
	player.GetCamera()->UpdateAspectRatio((float)width / (float)height);
	player.SetScreenSize(width, height);
	hudCamera.UpdateSize((float)width, (float)height);

	// Initialize world
	world.Generate(m_deviceResources.get(),mode);


	if (mode != GAME) {
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot3D::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(m_deviceResources.get()->GetD3DDevice(), m_deviceResources.get()->GetD3DDeviceContext());
	}
}

/// <summary>
/// Updates the game, then renders it
/// </summary>
void Game::Tick() {
	// DX::StepTimer will compute the elapsed time and call Update() for us
	// We pass Update as a callback to Tick() because StepTimer can be set to a "fixed time" step mode, allowing us to call Update multiple time in a row if the framerate is too low (useful for physics stuffs)
	m_timer.Tick([&]() { Update(m_timer); });

	Render(m_timer);
}

/// <summary>
/// Updates the game's
/// </summary>
/// <param name="timer">The game's timer</param>
void Game::Update(DX::StepTimer const& timer) {

	auto const kb = m_keyboard->GetState();
	auto const ms = m_mouse->GetState();
	m_mouse->ResetScrollWheelValue();
	
	player.Update(timer.GetElapsedSeconds(), kb, ms);

	if (kb.Escape)
		ExitGame();

	if (m_mode != GAME) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Im(timer);
	}

	auto const pad = m_gamePad->GetState(0);
}

/// <summary>
/// Draws the scene
/// </summary>
/// <param name="timer">The game's timer</param>
void Game::Render(DX::StepTimer const& timer) {
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return;

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();
	auto const viewport = m_deviceResources->GetScreenViewport();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//ApplyInputLayout<VertexLayout_PositionNormalUV>(m_deviceResources.get());
	ApplyInputLayout<VertexLayout_PositionNormalUVInstanced>(m_deviceResources.get());

	// Draw Skybox

	player.GetCamera()->ApplyCamera(m_deviceResources.get());
	world.GetScene()->DrawSkybox(m_deviceResources.get());
	
	// Draw World
	blockShader.Apply(m_deviceResources.get());
	player.GetCamera()->ApplyCamera(m_deviceResources.get());
	world.Draw(player.GetCamera(), m_deviceResources.get());

	world.GetScene()->ApplyDirectionalLight(m_deviceResources.get());
	player.Draw(m_deviceResources.get());
	

	// Draw UI

	hudCamera.ApplyCamera(m_deviceResources.get());
	ApplyInputLayout<VertexLayout_PositionColor>(m_deviceResources.get());
	basicShader.Apply(m_deviceResources.get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->Draw(4, 0);


	if (m_mode != GAME) {
		// Render ImGui
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}


	// Render All
	m_deviceResources->Present();
}


#pragma region Message Handlers
void Game::OnActivated() {}

void Game::OnDeactivated() {}

void Game::OnSuspending() {}

void Game::OnResuming() {
	m_timer.ResetElapsedTime();
}

void Game::OnWindowMoved() {
	auto const r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange() {
	m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height) {
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	// The windows size has changed:
	// We can realloc here any resources that depends on the target resolution (post processing etc)
	player.SetScreenSize(width, height);
	player.GetCamera()->UpdateAspectRatio((float)width / (float)height);
	hudCamera.UpdateSize((float)width, (float)height);
}

void Game::OnDeviceLost() {
	// We have lost the graphics card, we should reset resources [TODO]
}

void Game::OnDeviceRestored() {
	// We have a new graphics card context, we should realloc resources [TODO]
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept {
	width = 1280;
	height = 720;
}

#pragma endregion


/// <summary>
/// Represents the ImGui UI
/// </summary>
/// <param name="timer"></param>
void Game::Im(DX::StepTimer const& timer)
{
	if (m_mode == GAME_DEBUG) {
		ImGui::Begin("Debug", NULL);
		ImGui::SetWindowCollapsed(false);
		ImGui::SetWindowSize(ImVec2(500, 600), ImGuiCond_Always);

		if (ImGui::CollapsingHeader("Framerate")) {
			ImGui::Text("FPS : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(timer.GetFramesPerSecond()).c_str());
			ImGui::Text("Time between two frames : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(timer.GetElapsedSeconds() * 1000.0f).c_str());
		}

		ImGui::End();
	}
	else if (m_mode == SCENE_EDITOR) {
		world.GetScene()->Im();
	}

}