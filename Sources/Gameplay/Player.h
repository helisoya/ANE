#pragma once

#include "Engine/DepthState.h"
#include "Engine/Camera.h"
#include "Engine/StepTimer.h"
#include "Gameplay/World.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// Represents the player
/// </summary>
class Player {
	World* world = nullptr;

	Vector3 position = Vector3();
	Vector3 defaultPosition;

	float walkSpeed = 10.0f;

	PerspectiveCamera camera = PerspectiveCamera(75, 1);

	float currentYaw = 0;

	DirectX::Mouse::ButtonStateTracker      mouseTracker;
	DirectX::Keyboard::KeyboardStateTracker keyboardTracker;

	int screenWidth = 800;
	int screenHeight = 600;

	GameMode mode = GAME;

public:
	Player(World* w, Vector3 pos) : world(w), position(pos), defaultPosition(pos){}

	/// <summary>
	/// Generates the player's resources
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="mode">The game's mode</param>
	void GenerateGPUResources(DeviceResources* deviceRes, GameMode mode);

	/// <summary>
	/// Updates the player
	/// </summary>
	/// <param name="dt">The delta time</param>
	/// <param name="kb">The keyboard's state</param>
	/// <param name="ms">The mouse's state</param>
	void Update(float dt, DirectX::Keyboard::State kb, DirectX::Mouse::State ms);

	/// <summary>
	/// Draws the player
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Draw(DeviceResources* deviceRes);

	// Resets the player
	void Reset();

	/// <summary>
	/// Changes the screen's size for the player
	/// </summary>
	/// <param name="width">The screen width</param>
	/// <param name="height">The screen height</param>
	void SetScreenSize(int width, int height);

	// ImGui pass for the player
	void Im(DX::StepTimer const& timer);

	// Gets the player's camera
	PerspectiveCamera* GetCamera() { return &camera; }
};
