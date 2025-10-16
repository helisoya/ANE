#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ANEInput.h"
#include "ANEApplication.h"

class ANESystem
{
public:
	ANESystem();
	ANESystem(const ANESystem& system);
	~ANESystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	ANEInput* m_Input;
	ANEApplication* m_Application;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static ANESystem* ApplicationHandle = 0;

