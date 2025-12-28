#pragma once

#include "Engine/BlendState.h"
#include "Engine/Camera.h"
#include "Gameplay/GameModel.h"
#include "Engine/Texture.h"
#include "GamePlay/Material.h"
#include "Gameplay/Scene.h"
#include "Game.h"



/// <summary>
///  Represents the world (Scene, Skybox, Characters, ...)
/// </summary>
class World {

	DeviceResources* deviceRes;

	Scene scene;

public:
	World();
	virtual ~World();

	/// <summary>
	/// Generates the world using procedural generation
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="mode">The current game mode</param>
	void Generate(DeviceResources* deviceRes, GameMode mode);

	/// <summary>
	/// Draws the world
	/// </summary>
	/// <param name="camera">The game's camera</param>
	/// <param name="deviceRes">The game's device resources</param>
	void Draw(Camera* camera, DeviceResources* deviceRes);

	// Reset the world
	void Reset();

	// Gets the scene
	Scene* GetScene();

private:

	/// <summary>
	/// Creates the world
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Create(DeviceResources* deviceRes);
};
