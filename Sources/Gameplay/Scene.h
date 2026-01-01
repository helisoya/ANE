#pragma once

#include "Engine/BlendState.h"
#include "Engine/Camera.h"
#include "Gameplay/GameModel.h"
#include "Engine/Texture.h"
#include "GamePlay/Material.h"
#include "Game.h"
#include "Gameplay/SceneEditorData.h"
#include "Engine/Light.h"
#include "Gameplay/Skybox.h"
#include "Gameplay/Interaction.h"


/// <summary>
/// A scene contains the different materials/models/entities that compose the background
/// </summary>
class Scene {

	std::vector<Material> materials;
	std::vector<Interaction> interactions;

	Skybox skybox;

	Light directionalLight;

	std::wstring currentId;

	GameMode mode;
	SceneEditorData* editorData;
	DeviceResources* deviceRes;

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

	/// <summary>
	/// Draws the skybox
	/// </summary>
	/// <param name="deviceRes">The device resources</param>
	void DrawSkybox(DeviceResources* deviceRes);

	/// <summary>
	/// Applies the skybox's data
	/// </summary>
	/// <param name="deviceRes">The skybox's data</param>
	void ApplySkybox(DeviceResources* deviceRes);

	/// <summary>
	/// Applies the directional light's data
	/// </summary>
	/// <param name="deviceRes">The device resources</param>
	void ApplyDirectionalLight(DeviceResources* deviceRes);

	/// <summary>
	/// Changes the directional light direction
	/// </summary>
	/// <param name="newDirection">The new direction</param>
	void ChangeLightDirection(Vector3 newDirection);

	/// <summary>
	/// Gets the directional light
	/// </summary>
	/// <returns>The directional light</returns>
	Light& GetLight();

	/// <summary>
	/// Sets the skybox's texture
	/// </summary>
	/// <param name="newTexture">The new texture</param>
	void SetSkyboxTexture(std::wstring newTexture);

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
	/// Loads the default debug scene
	/// </summary>
	void LoadDebugScene();

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
	/// Adds a new interaction point to the scene
	/// </summary>
	/// <param name="name">The interaction's name</param>
	/// <returns>The interaction</returns>
	Interaction* AddInteraction(std::wstring name);

	/// <summary>
	/// Gets's an interaction point from the scene
	/// </summary>
	/// <param name="name">The interaction's name</param>
	/// <returns>The interaction</returns>
	Interaction* GetInteraction(std::wstring name);

	/// <summary>
	/// Removes an interaction from the scene
	/// </summary>
	/// <param name="name">The interaction's name</param>
	void RemoveInteraction(const std::wstring& name);

	/// <summary>
	/// Removes an entity from a model
	/// </summary>
	/// <param name="materialId">The linked material</param>
	/// <param name="modelId">The model's ID</param>
	/// <param name="id">The Entity's ID</param>
	void RemoveEntity(const std::wstring& materialId,const std::wstring& modelId, const std::wstring& id);

	/// <summary>
	/// Removes a model from a material
	/// </summary>
	/// <param name="materialId">The linked material</param>
	/// <param name="modelId">The model's ID</param>
	void RemoveModel(const std::wstring& materialId, const std::wstring& modelId);

	/// <summary>
	/// Removes a material
	/// </summary>
	/// <param name="materialId">The material</param>
	void RemoveMaterial(const std::wstring& materialId);

	/// <summary>
	/// Refreshs all instance buffers
	/// </summary>
	void RefreshInstanceBuffers();

	/// <summary>
	/// Updates the ImGui UI (Scene Editor)
	/// </summary>
	void Im();

private:
	/// <summary>
	/// Regenerates the scene editor data
	/// </summary>
	void RegenerateSceneEditorData();

	/// <summary>
	/// Clears the current scene editor's data
	/// </summary>
	void ClearCurrentSceneEditorDataContent();
};
