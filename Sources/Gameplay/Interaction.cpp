#include "pch.h"

#include "Interaction.h"

Interaction::Interaction(std::wstring name)
	: name(name), scale(1,1,1), linkedEntityId(L"_"),linkedMaterialId(L"_"),linkedModelId(L"_"),isLinkedToEntity(false),
nextChapter(L"Chapter"){
	RefreshBoudingBox();
}



void Interaction::SetName(const std::wstring& id)
{
	this->name = id;
}

void Interaction::SetPosition(const Vector3& position) {
	this->position = position;
	RefreshBoudingBox();
}


void Interaction::SetScale(const Vector3& scale)
{
	this->scale = scale;
	RefreshBoudingBox();
}

void Interaction::SetIsInteractable(const bool& interactable)
{
	this->interactable = interactable;
}

void Interaction::SetNextChapter(const std::wstring& nextChapter)
{
	this->nextChapter = nextChapter;
}

void Interaction::SetLinkToEntity(const bool& isLinked, const std::wstring& materialId, 
	const std::wstring& modelId, const std::wstring& entityId)
{
	this->isLinkedToEntity = isLinked;
	this->linkedMaterialId = materialId;
	this->linkedModelId = modelId;
	this->linkedEntityId = entityId;
}


const std::wstring& Interaction::GetLinkedEntityID()
{
	return linkedEntityId;
}

const std::wstring& Interaction::GetLinkedMaterialId()
{
	return linkedMaterialId;
}

const std::wstring& Interaction::GetLinkedModelId()
{
	return linkedModelId;
}

const bool& Interaction::IsLinkedToEntity()
{
	return isLinkedToEntity;
}

const bool& Interaction::IsInteractable()
{
	return interactable;
}

const std::wstring& Interaction::GetNextChapter()
{
	return nextChapter;
}

const std::wstring& Interaction::GetName()
{
	return name;
}

const Vector3& Interaction::GetPosition()
{
	return position;
}

const Vector3& Interaction::GetScale()
{
	return scale;
}

const BoundingBox& Interaction::GetBoundingBox()
{
	return boundingBox;
}

void Interaction::RefreshBoudingBox()
{
	Vector3 dividedByTwo = scale / 2.0f;

	boundingBox.Center = position + dividedByTwo; 
	boundingBox.Extents = dividedByTwo;
}
