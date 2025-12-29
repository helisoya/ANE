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
	scene.Generate(deviceRes, mode);

	Reset();

	if (mode == SCENE_EDITOR) {
		scene.LoadDebugScene();
	}

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
