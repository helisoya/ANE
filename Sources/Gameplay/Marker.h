#pragma once

#include <iostream>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// Represents an entity (instance of a game model)
/// </summary>
class Marker {
private:
	std::wstring name;

	Vector3 position;
	Quaternion rotation;

public:

	Marker(std::wstring name);

	void SetName(const std::wstring& name);
	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);

	const std::wstring& GetName();
	const Vector3& GetPosition();
	const Vector3& GetRotation();

};