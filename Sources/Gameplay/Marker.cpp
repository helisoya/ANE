#include "pch.h"

#include "Marker.h"

Marker::Marker(std::wstring name)
	: name(name), rotation(Quaternion::Identity){
}



void Marker::SetName(const std::wstring& id)
{
	this->name = id;
}

void Marker::SetPosition(const Vector3& position) {
	this->position = position;
}


void Marker::SetRotation(const Vector3& rotation)
{
	this->rotation = Quaternion::CreateFromYawPitchRoll(rotation);
}

const std::wstring& Marker::GetName()
{
	return name;
}

const Vector3& Marker::GetPosition()
{
	return position;
}

const Vector3& Marker::GetRotation()
{
	return rotation.ToEuler();
}