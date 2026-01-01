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

	std::wstring id;

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
	/// <param name="useInstancing">Allow instancing (False will draw all models one by one)</param>
	void Draw(DeviceResources* deviceRes, bool allowInstancing = false);

	/// <summary>
	/// Adds a model to the material
	/// </summary>
	/// <param name="modelID">The model's ID</param>
	/// <param name="deviceRes">The game's device resources</param>
	GameModel* AddModel(std::wstring modelID, DeviceResources* deviceRes);

	/// <summary>
	/// Gets a model from the material's list
	/// </summary>
	/// <param name="modelId">The model's ID</param>
	/// <returns>The model</returns>
	GameModel* GetModel(std::wstring modelId);

	/// <summary>
	/// Get all linked models
	/// </summary>
	/// <returns>Every models linked to this material</returns>
	std::vector<GameModel>& GetModels();

	/// <summary>
	/// Regenerates the models instance buffers
	/// </summary>
	/// /// <param name="deviceRes">The game's device resources</param>
	void RegenerateModelsInstanceBuffers(DeviceResources* deviceRes);

	/// <summary>
	/// Removes an entity from a model's list
	/// </summary>
	/// <param name="modelId">The model's ID</param>
	/// <param name="entityId">The entity's ID</param>
	/// <returns>True if the material has no linked model</returns>
	bool RemoveEntity(const std::wstring& modelId, const std::wstring& entityId);

	/// <summary>
	/// Removes an entire model from the material's list
	/// </summary>
	/// <param name="modelId">The model's ID</param>
	/// <returns>True if the material has no linked model</returns>
	bool RemoveModel(const std::wstring& modelId);

	/// <summary>
	/// Release the material
	/// </summary>
	void Release();

	/// <summary>
	/// Gets the Material's ID
	/// </summary>
	/// <returns>The material's ID</returns>
	const std::wstring& GetID();

private:

};