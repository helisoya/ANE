#pragma once

#include "ANED3D.h"
#include "ANECamera.h"
#include "ANEModel.h"
#include "ANEShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ANEApplication
{
	public:
		ANEApplication();
		ANEApplication(const ANEApplication& application);
		~ANEApplication();

		bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
		void Shutdown();
		bool Frame();

	private:
		bool Render();

	private:
		ANED3D* m_Direct3D;
		ANECamera* m_Camera;
		ANEModel* m_Model;
		ANEShader* m_ColorShader;
};

