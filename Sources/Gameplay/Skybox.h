#pragma once

#include "Engine/VertexLayout.h"
#include "Engine/Buffers.h"
#include "Engine/Shader.h"
#include "Engine/Texture.h"


/// <summary>
/// Represents the game's skybox
/// </summary>
class Skybox {
public:

private:
	VertexBuffer<VertexLayout_PositionNormalUV> vertexBuffer;
	IndexBuffer indexBuffer;

	Shader<VertexLayout_PositionNormalUV> shader;
	Texture texture;

public:

	Skybox();

	/// <summary>
	/// Generates the skybox
	/// </summary>
	/// <param name="deviceResources">The game's device resources</param>
	void Generate(DeviceResources* deviceResources);

	/// <summary>
	/// Changes the skybox's texture
	/// </summary>
	/// <param name="texture">The new texture</param>
	/// <param name="deviceResources">The game's device resources</param>
	void ChangeTexture(std::wstring texture, DeviceResources* deviceResources);
	
	/// <summary>
	/// Gets the skybox's texture
	/// </summary>
	/// <returns>The new skybox</returns>
	const std::wstring& GetTextureName();

	/// <summary>
	/// Pushs a face on the skybox
	/// </summary>
	/// <param name="pos">The face's position</param>
	/// <param name="up">The face's up vector</param>
	/// <param name="right">The face's right vector</param>
	/// <param name="uvIdx">The face's UV Index</param>
	void PushFace(Vector3 pos, Vector3 up, Vector3 right, int uvIdx);

	/// <summary>
	/// Draws the skybox
	/// </summary>
	/// <param name="deviceResources">The game's device resources</param>
	void Draw(DeviceResources* deviceResources);

	/// <summary>
	/// Applies the skybox's data
	/// </summary>
	/// <param name="deviceResources">The game's device resources</param>
	void Apply(DeviceResources* deviceResources);

private:

	Vector4 ToVec4(const Vector3& v);
};