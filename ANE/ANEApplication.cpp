#include "ANEApplication.h"

#include <iostream>

ANEApplication::ANEApplication()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Shader = 0;
	m_mode = GAME;
}

ANEApplication::~ANEApplication()
{
}

bool ANEApplication::Initialize(int screenWidth, int screenHeight, HWND hwnd, ApplicationMode mode)
{
	bool result;

	m_mode = mode;

	// Create and initialize the Direct3D object.
	m_Direct3D = new ANED3D;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new ANECamera;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	m_Model = new ANEModel;

	char textureFilename[128];
	strcpy_s(textureFilename, "../ANE/resources/textures/debug.tga");
	result = m_Model->Initialize(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext(), textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the color shader object.
	m_Shader = new ANEShader(L"default");

	result = m_Shader->Initialize(m_Direct3D->GetDevice(), 
		hwnd,VertexLayout_PositionUV::InputElementDescs.data(), 
		VertexLayout_PositionUV::InputElementDescs.size());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	
	if (m_mode != GAME) {
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	}

	return true;
}

void ANEApplication::Shutdown()
{
	// Release the color shader object.
	if (m_Shader)
	{
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (m_mode != GAME) {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

bool ANEApplication::Frame()
{
	m_timer.Tick([&]() { Update(); });

	Render();

	return true;
}

bool ANEApplication::Render()
{
	if (m_timer.GetFrameCount() == 0)
		return true;


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_Shader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	if (m_mode != GAME) {
		// Render ImGui
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return true;
}

bool ANEApplication::Update()
{
	double deltaTime = m_timer.GetElapsedSeconds();

	if (m_mode != GAME) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Im();
	}

	return true;
}

void ANEApplication::Im()
{
	ImGui::Begin("ANE");
	ImGui::SetWindowSize(ImVec2(500, 600));
	
	if (m_mode == GAME_DEBUG) {
		ImGui::Text("FPS : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_timer.GetFramesPerSecond()).c_str());
	}
	else if (m_mode == SCENE_EDITOR) {
		if (ImGui::CollapsingHeader("Scenes")) {
			for (int i = 0; i < 2; i++) {
				if (ImGui::Button(std::to_string(i).c_str())) {

				}
			}

			ImGui::Spacing();

			ImGui::Text("Scene Name");
			ImGui::SameLine();
			if (ImGui::Button("Create scene")) {

			}
		}
	}
	ImGui::End();
}
