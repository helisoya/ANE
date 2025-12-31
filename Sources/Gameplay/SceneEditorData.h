#pragma once

#include "GameEntity.h"
#include "GameModel.h"
#include "Engine/Texture.h"
#include "Interaction.h"
#include <map>

struct SceneEditorEntityData {
	char name[64] = "Object";
	float position[3] = { 0,0,0 };
	float rotation[3] = { 0,0,0 };
	float scale[3] = { 1.0f ,1.0f,1.0f };
};

struct SceneEditorInteractionData {
	char name[64] = "Object";
	bool interactable = true;

	bool isLinkedToEntity = false;
	char entityLinked[64] = "ENT";
	char modelLinked[64] = "MDL";
	char materialLinked[64] = "MAT";

	float position[3] = { 0,0,0 };
	float scale[3] = { 1.0f ,1.0f,1.0f };
	GameEntity* previewEntity;
};

struct SceneEditorContentData {

public:
	char sceneId[64] = "Test";
	char skyBox[64] = "skybox";
	float lightDirection[3] = { 0,0,0 };


	std::map<GameEntity*, SceneEditorEntityData*> entities;
	std::map<Interaction*, SceneEditorInteractionData*> interactions;

};

struct SceneEditorData {

public:

	std::vector<std::string> scenes;

	SceneEditorContentData* content;

	bool drawEntities = true;
	bool drawMarkers = true;
	bool drawInteractions = true;
	bool useInstancing = false;

	GameModel* interactionsModel;
	Texture* interactionsTexture;

	char newObjectMaterial[64] = "Default";
	char newObjectModel[64] = "Crate";
	char newInteraction[64] = "Interaction";
};



