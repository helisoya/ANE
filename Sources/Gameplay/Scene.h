#pragma once

#include "Engine/BlendState.h"
#include "Engine/Camera.h"
#include "Gameplay/GameModel.h"
#include "Engine/Texture.h"
#include "GamePlay/Material.h"
#include "Game.h"
#include "Gameplay/SceneEditorData.h"



/// <summary>
/// A scene contains the different materials/models/entities that compose the background
/// </summary>
class Scene {

	std::vector<Material> materials;

	std::wstring currentId;

	GameMode mode;
	SceneEditorData* editorData;

public:
	Scene();
	virtual ~Scene();

	/// <summary>
	/// Generates the world using procedural generation
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="mode">The current game mode</param>
	void Generate(DeviceResources* deviceRes, GameMode mode);

	/// <summary>
	/// Draws the world
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Draw(DeviceResources* deviceRes);

	// Reset the scene
	void Reset();

	// Saves the scene
	void SaveScene();

	/// <summary>
	/// Loads a scene from file
	/// </summary>
	/// <param name="filepath">The scene filepath</param>
	void LoadScene(std::wstring filepath);

	/// <summary>
	/// Deletes the current scene from disk
	/// </summary>
	void DeleteSceneFromDisk();

	/// <summary>
	/// Gets the scene's materials
	/// </summary>
	/// <returns>The scene materials</returns>
	const std::vector<Material>& GetMaterials();

	/// <summary>
	/// Adds a material to the scene
	/// </summary>
	/// <param name="materialID">The material's ID</param>
	/// <param name="deviceRes">The game's device resources</param>
	Material* AddMaterial(std::wstring materialID, DeviceResources* deviceRes);

	/// <summary>
	/// Gets a material from the scene's list
	/// </summary>
	/// <param name="materialId">The material's ID</param>
	/// <returns>The material</returns>
	Material* GetMaterial(std::wstring materialId);

	/// <summary>
	/// Updates the ImGui UI (Scene Editor)
	/// </summary>
	void Im();

private:
	/// <summary>
	/// Regenerates the scene editor data
	/// </summary>
	void RegenerateSceneEditorData();
};
