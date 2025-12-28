#include "pch.h"

#include "Engine/DefaultResources.h"
#include "World.h"
#include "PerlinNoise.hpp"
#include "iostream"
#include "fstream"
#include "sstream"

World::World() : defaultTexture(L"Default") {
}

World::~World() {

}

void World::Generate(DeviceResources* deviceRes) {
	
	this->deviceRes = deviceRes;

	Reset();

	model.LoadFromOBJ(L"Models/Scenes/Crate.obj",deviceRes);
	defaultTexture.Create(deviceRes);

	Create(deviceRes);
}


void World::Draw(Camera* camera, DeviceResources* deviceRes) {
	auto gpuRes = DefaultResources::Get();
	gpuRes->cbModel.ApplyToVS(deviceRes, 0);

	gpuRes->opaque.Apply(deviceRes);
	gpuRes->defaultDepth.Apply(deviceRes);

	defaultTexture.Apply(deviceRes);
	gpuRes->cbModel.data.model = model.model.Transpose();
	gpuRes->cbModel.data.isInstance = false;
	gpuRes->cbModel.UpdateBuffer(deviceRes);
	model.Draw(deviceRes);

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
