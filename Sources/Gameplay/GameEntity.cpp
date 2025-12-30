#include "pch.h"

#include "GameEntity.h"
#include "GameModel.h"

GameEntity::GameEntity(std::wstring name, std::wstring modelId, std::wstring materialId)
	: name(name), scale(1,1,1), modelId(modelId), materialId(materialId) {
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

void GameEntity::SetScale(const Vector3& scale)
{
	this->scale = scale;
	RefreshWorldMatrix();
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

const Vector3& GameEntity::GetScale()
{
	return scale;
}

const Matrix& GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}

void GameEntity::RefreshWorldMatrix()
{
	worldMatrix = Matrix::CreateFromQuaternion(rotation) * Matrix::CreateScale(scale) * Matrix::CreateTranslation(position);
}
