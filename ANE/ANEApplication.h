#pragma once

#include "ANED3D.h"
#include "ANECamera.h"
#include "ANEModel.h"
#include "ANEShader.h"
#include "ANEVertexLayout.h"
#include "ANEStepTimer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

enum ApplicationMode {
	GAME,
	GAME_DEBUG,
	SCENE_EDITOR
};


class ANEApplication
{
	public:
		ANEApplication();
		~ANEApplication();

		bool Initialize(int screenWidth, int screenHeight, HWND hwnd, ApplicationMode mode);
		void Shutdown();
		bool Frame();

	private:
		bool Render();
		bool Update();

		void Im();

	private:
		ANED3D* m_Direct3D;
		ANECamera* m_Camera;
		ANEModel* m_Model;
		ANEShader* m_Shader;

		ANEStepTimer m_timer;

		ApplicationMode m_mode;
};


