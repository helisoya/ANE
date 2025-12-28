#include "pch.h"

#include "GameEntity.h"
#include "GameModel.h"

GameEntity::GameEntity(std::wstring id) : id(id), scale(1), interactable(false) {
	RefreshWorldMatrix();
}


void GameEntity::SetID(const std::wstring& id)
{
	this->id = id;
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

const std::wstring& GameEntity::GetID()
{
	return id;
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
	worldMatrix = Matrix::CreateTranslation(position) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateScale(scale);
}
