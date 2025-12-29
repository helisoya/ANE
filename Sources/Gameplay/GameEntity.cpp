#include "pch.h"

#include "GameEntity.h"
#include "GameModel.h"

GameEntity::GameEntity(std::wstring name, USHORT id, std::wstring modelId, std::wstring materialId)
	: name(name), id(id), scale(1), interactable(false), modelId(modelId),materialId(materialId) {
	RefreshWorldMatrix();
}



void GameEntity::SetName(const std::wstring& id)
{
	this->name = id;
}

void GameEntity::SetPosition(const Vector3& position) {
	this->position = position;
	RefreshWorldMatrix();
}

void GameEntity::SetRotation(const Vector3& rotation) {
	this->rotation = Quaternion::CreateFromYawPitchRoll(rotation);
	RefreshWorldMatrix();
}

void GameEntity::SetScale(const float& scale)
{
	this->scale = scale;
	RefreshWorldMatrix();
}

void GameEntity::SetInteractable(const bool& interactable)
{
	this->interactable = interactable;
}

const USHORT& GameEntity::GetID() {
	return id;
}

const std::wstring& GameEntity::GetMaterialId()
{
	return materialId;
}

const std::wstring& GameEntity::GetModelId()
{
	return modelId;
}

const std::wstring& GameEntity::GetName()
{
	return name;
}

const Vector3& GameEntity::GetPosition()
{
	return position;
}

const Vector3& GameEntity::GetRotation()
{
	return rotation.ToEuler();
}

const float& GameEntity::GetScale()
{
	return scale;
}

const bool& GameEntity::IsInteractable()
{
	return interactable;
}

const Matrix& GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

void GameEntity::RefreshWorldMatrix()
{
	worldMatrix = Matrix::CreateFromQuaternion(rotation) *Matrix::CreateScale(scale) * Matrix::CreateTranslation(position);
}
