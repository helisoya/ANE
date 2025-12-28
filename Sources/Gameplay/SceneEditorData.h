#pragma once

#include "GameEntity.h"
#include <map>

struct SceneEditorEntityData {
	char id[64] = "Object";
	float position[3] = { 0,0,0 };
	float rotation[3] = { 0,0,0 };
	float scale = 1.0f;
	bool interactable = true;
};

struct SceneEditorData {

public:
	char sceneId[64] = "Test";
	char skyBox[64] = "skybox";
	float lightDirection[3] = { 0,0,0 };

	std::map<GameEntity*, SceneEditorEntityData*> entities;
};


