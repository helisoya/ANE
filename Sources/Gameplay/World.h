#pragma once

#include "Engine/BlendState.h"
#include "Engine/Camera.h"
#include "Gameplay/GameModel.h"
#include "Engine/Texture.h"
#include "GamePlay/Material.h"



/// <summary>
///  Represents the world
/// </summary>
class World {

	DeviceResources* deviceRes;

	Material defaultMaterial;

public:
	World();
	virtual ~World();

	/// <summary>
	/// Generates the world using procedural generation
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Generate(DeviceResources* deviceRes);

	/// <summary>
	/// Draws the world
	/// </summary>
	/// <param name="camera">The game's camera</param>
	/// <param name="deviceRes">The game's device resources</param>
	void Draw(Camera* camera, DeviceResources* deviceRes);

	// Reset the world
	void Reset();

private:

	/// <summary>
	/// Creates the world
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Create(DeviceResources* deviceRes);
};
