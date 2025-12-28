#include "pch.h"

#include "Engine/DefaultResources.h"
#include "World.h"
#include "PerlinNoise.hpp"
#include "iostream"
#include "fstream"
#include "sstream"

World::World() : scene() {
}

World::~World() {

}

void World::Generate(DeviceResources* deviceRes, GameMode mode) {
	
	this->deviceRes = deviceRes;

	Reset();

	Material* material = scene.AddMaterial(L"Default",deviceRes);
	GameModel* crateModel = material->AddModel(L"Crate", deviceRes);
	crateModel->AddEntity(L"Obj1");
	crateModel->AddEntity(L"Obj2")->SetPosition(Vector3(5, 0, 0));
	crateModel->GetEntity(L"Obj1")->SetScale(2);

	scene.Generate(deviceRes, mode);

	Create(deviceRes);
}


void World::Draw(Camera* camera, DeviceResources* deviceRes) {
	auto gpuRes = DefaultResources::Get();
	gpuRes->cbModel.ApplyToVS(deviceRes, 0);

	gpuRes->opaque.Apply(deviceRes);
	gpuRes->defaultDepth.Apply(deviceRes);

	scene.Draw(deviceRes);

	// Clean

	gpuRes->cbModel.data.model = Matrix::Identity;
	gpuRes->cbModel.UpdateBuffer(deviceRes);
}


void World::Reset()
{

}

Scene* World::GetScene()
{
	return &scene;
}




void World::Create(DeviceResources* deviceRes)
{
	DefaultResources::Get()->cbModel.Create(deviceRes);
}
