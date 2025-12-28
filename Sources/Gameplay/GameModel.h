#pragma once

#include "Engine/Buffers.h"
#include "Engine/VertexLayout.h"

/// <summary>
/// Represents a 3D Model
/// </summary>
class GameModel {
private:
	VertexBuffer<VertexLayout_PositionNormalUV> vb;
	VertexBuffer<Vector3> instbuffer;
	IndexBuffer ib;

public:
	Matrix model = Matrix::Identity;

	GameModel() {}

	/// <summary>
	/// Loads the model from an .OBJ
	/// </summary>
	/// <param name="filePath">The OBJ's filepath</param>
	/// /// <param name="deviceRes">The game's device resources</param>
	void LoadFromOBJ(std::wstring filePath, DeviceResources* deviceRes);


	/// <summary>
	/// Draws the model
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="isInstanced">Use instancing</param>
	void Draw(DeviceResources* deviceRes, bool isInstanced = false);

	/// <summary>
	/// Resets the instance buffer's positions
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	/// <param name="positions">The instanced positions</param>
	void ResetInstanceBuffer(DeviceResources* deviceRes, std::vector<Vector3>* positions);

private:

	/// <summary>
	/// Pushs a triangle to the model's buffers
	/// </summary>
	/// <param name="a">Point A's position</param>
	/// <param name="b">Point B's position</param>
	/// <param name="c">Point C's position</param>
	/// <param name="normal">The face's normal</param>
	/// <param name="uva">Point A's UV</param>
	/// <param name="uvb">Point B's UV</param>
	/// <param name="uvc">Point C's UV</param>
	/// <param name="front">True if it is a front face</param>
	void PushTriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, Vector2 uva, Vector2 uvb, Vector2 uvc, bool front = true);
};