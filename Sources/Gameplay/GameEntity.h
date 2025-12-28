#pragma once

#include <iostream>

using namespace DirectX::SimpleMath;

/// <summary>
/// Represents an entity (instance of a game model)
/// </summary>
class GameEntity {
private:
	std::wstring id;

	Vector3 position;
	Quaternion rotation;
	float scale;

	bool interactable;
	Matrix worldMatrix;

public:

	GameEntity(std::wstring id);

	void SetID(const std::wstring& id);
	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);
	void SetScale(const float& scale);
	void SetInteractable(const bool& interactable);

	const std::wstring& GetID();
	const Vector3& GetPosition();
	const Vector3& GetRotation();
	const float& GetScale();
	const bool& IsInteractable();

	const Matrix& GetWorldMatrix();
private:
	void RefreshWorldMatrix();
};