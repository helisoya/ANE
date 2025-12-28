#include "pch.h"

#include "Engine/DefaultResources.h"
#include "World.h"
#include "PerlinNoise.hpp"
#include "iostream"
#include "fstream"
#include "sstream"

World::World() : defaultMaterial(L"Default") {
}

World::~World() {

}

void World::Generate(DeviceResources* deviceRes) {
	
	this->deviceRes = deviceRes;

	Reset();

	defaultMaterial.Create(deviceRes);
	GameModel* crateModel = defaultMaterial.AddModel(L"Crate", deviceRes);
	crateModel->AddEntity(L"Obj1");
	crateModel->AddEntity(L"Obj2")->SetPosition(Vector3(5, 0, 0));
	crateModel->GetEntity(L"Obj1")->SetScale(2);

	Create(deviceRes);
}


void World::Draw(Camera* camera, DeviceResources* deviceRes) {
	auto gpuRes = DefaultResources::Get();
	gpuRes->cbModel.ApplyToVS(deviceRes, 0);

	gpuRes->opaque.Apply(deviceRes);
	gpuRes->defaultDepth.Apply(deviceRes);

	defaultMaterial.Draw(deviceRes,false);

	// Clean

	gpuRes->cbModel.data.model = Matrix::Identity;
	gpuRes->cbModel.UpdateBuffer(deviceRes);
}


void World::Reset()
{

}




void World::Create(DeviceResources* deviceRes)
{

	DefaultResources::Get()->cbModel.Create(deviceRes);
}
