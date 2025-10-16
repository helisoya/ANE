#include "ANEApplication.h"

ANEApplication::ANEApplication()
{
	m_Direct3D = 0;
}

ANEApplication::ANEApplication(const ANEApplication& application)
{
}

ANEApplication::~ANEApplication()
{
}

bool ANEApplication::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create and initialize the Direct3D object.
	m_Direct3D = new ANED3D;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ANEApplication::Shutdown()
{
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool ANEApplication::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool ANEApplication::Render()
{
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
