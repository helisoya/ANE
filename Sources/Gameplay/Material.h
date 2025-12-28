#pragma once

#include "Engine/Buffers.h"
#include "Engine/VertexLayout.h"
#include "Engine/Texture.h"
#include "GameModel.h"
#include <iostream>

/// <summary>
/// Represents a material
/// </summary>
class Material {
private:
	Texture texture;
	std::vector<GameModel> gameModels;

public:

	Material(std::wstring texturePath);

	/// <summary>
	/// Creates the material
	/// </summary>
	/// /// <param name="deviceRes">The game's device resources</param>
	void Create(DeviceResources* deviceRes);


	/// <summary>
	/// Draws the models linked to this material
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="useInstancing">Use instancing (False will draw all models one by one)</param>
	void Draw(DeviceResources* deviceRes, bool useInstancing = false);

	/// <summary>
	/// Adds a model to the material
	/// </summary>
	/// <param name="modelID">The model's ID</param>
	/// <param name="deviceRes">The game's device resources</param>
	GameModel* AddModel(std::wstring modelID, DeviceResources* deviceRes);

	GameModel* GetModel(std::wstring modelId);

private:

};