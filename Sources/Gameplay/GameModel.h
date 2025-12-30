#pragma once

#include "Engine/Buffers.h"
#include "Engine/VertexLayout.h"
#include "Gameplay/GameEntity.h"

/// <summary>
/// Represents a 3D Model
/// </summary>
class GameModel {
private:
	VertexBuffer<VertexLayout_PositionNormalUV> vb;
	VertexBuffer<Vector3> instbuffer;
	IndexBuffer ib;

	std::wstring id;
	std::wstring materialId;

	std::vector<GameEntity> entities;

public:

	/// <summary>
	/// Creates a cube
	/// </summary>
	/// <param name="deviceRes">The device resources</param>
	void CreateCube(DeviceResources* deviceRes);

	/// <summary>
	/// Creates a new empty game model
	/// </summary>
	/// <param name="deviceRes">The device resources</param>
	GameModel(DeviceResources* deviceRes);

	/// <summary>
	/// Creates a new game model
	/// </summary>
	/// <param name="id">The model ID</param>
	/// <param name="materialId">The material's ID</param>
	/// <param name="filePath">The model's filepath</param>
	/// <param name="deviceRes">The device resources</param>
	GameModel(std::wstring &id, std::wstring materialId, std::wstring &filePath, DeviceResources* deviceRes);

	/// <summary>
	/// Loads the model from an .OBJ
	/// </summary>
	/// <param name="filePath">The OBJ's filepath</param>
	/// /// <param name="deviceRes">The game's device resources</param>
	void LoadFromOBJ(std::wstring &filePath, DeviceResources* deviceRes);


	/// <summary>
	/// Draws the model
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="useInstancing">Use instancing (False will draw all models one by one)</param>
	void Draw(DeviceResources* deviceRes, bool useInstancing = false);

	/// <summary>
	/// Resets the instance buffer's positions
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void ResetInstanceBuffer(DeviceResources* deviceRes);

	/// <summary>
	/// Adds an entity to the list 
	/// </summary>
	/// <param name="name">The entity's name</param>
	GameEntity* AddEntity(std::wstring name);

	/// <summary>
	/// Gets an entity from the list
	/// </summary>
	/// <param name="name">The entity's name</param>
	/// <returns>The entity if it exists</returns>
	GameEntity* GetEntity(std::wstring name);

	/// <summary>
	/// Gets the entities of this model
	/// </summary>
	/// <returns>The entities</returns>
	std::vector<GameEntity>& GetEntities();

	/// <summary>
	/// Removes an entity from the list
	/// </summary>
	/// <param name="id">The entity's id</param>
	/// <returns>True if the model has no entities and should be deleted</returns>
	bool RemoveEntity(const std::wstring& name);

	/// <summary>
	/// Removes all entities from this model
	/// </summary>
	/// <param name="releaseAfterwards">True if the model's data should be purged too</param>
	void RemoveAllEntities(bool releaseAfterwards);

	/// <summary>
	/// Gets the Model's ID
	/// </summary>
	/// <returns>The model's ID</returns>
	const std::wstring& GetID();

	/// <summary>
	/// Release the model's buffers
	/// </summary>
	void Release();

private:

	/// <summary>
	/// Pushs a triangle to the model's buffers
	/// </summary>
	/// <param name="a">Point A's position</param>
	/// <param name="b">Point B's position</param>
	/// <param name="c">Point C's position</param>
	/// <param name="na">Point A's normal</param>
	/// <param name="nb">Point B's normal</param>
	/// <param name="nc">Point C's normal</param>
	/// <param name="uva">Point A's UV</param>
	/// <param name="uvb">Point B's UV</param>
	/// <param name="uvc">Point C's UV</param>
	/// <param name="front">True if it is a front face</param>
	void PushTriangle(Vector3 a, Vector3 b, Vector3 c, 
		Vector3 na, Vector3 nb, Vector3 nc,
		Vector2 uva, Vector2 uvb, Vector2 uvc, bool front = true);
};