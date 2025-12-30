#pragma once

#include <iostream>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// Represents an entity (instance of a game model)
/// </summary>
class Interaction {
private:
	std::wstring name;
	bool interactable;
	std::wstring nextChapter;

	bool isLinkedToEntity;
	std::wstring linkedModelId;
	std::wstring linkedMaterialId;
	std::wstring linkedEntityId;

	Vector3 position;
	Vector3 scale;

	BoundingBox boundingBox;

public:

	Interaction(std::wstring name);

	void SetName(const std::wstring& name);
	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetIsInteractable(const bool& interactable);
	void SetNextChapter(const std::wstring& nextChapter);
	void SetLinkToEntity(const bool& isLinked, const std::wstring& materialId, 
		const std::wstring& modelId, const std::wstring& entityId);

	const std::wstring& GetLinkedEntityID();
	const std::wstring& GetLinkedMaterialId();
	const std::wstring& GetLinkedModelId();
	const bool& IsLinkedToEntity();

	const bool& IsInteractable();
	const std::wstring& GetNextChapter();
	const std::wstring& GetName();
	const Vector3& GetPosition();
	const Vector3& GetScale();

	const BoundingBox& GetBoundingBox();
private:
	void RefreshBoudingBox();
};