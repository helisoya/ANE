#include "pch.h"

#include "Engine/DefaultResources.h"
#include "Scene.h"
#include "PerlinNoise.hpp"
#include "iostream"
#include "fstream"
#include "sstream"

Scene::Scene() : currentId(L"Test"), editorData(nullptr) {
}

Scene::~Scene() {
	std::map<GameEntity*, SceneEditorEntityData*>::iterator itr = editorData->entities.begin();
	if (itr != editorData->entities.end())
	{
		// found it - delete it
		delete itr->second;
		editorData->entities.erase(itr);
	}

	delete editorData;
}

void Scene::Generate(DeviceResources* deviceRes, GameMode mode) {

	mode = mode;
	if (mode == SCENE_EDITOR) {
		// Generate modification data
		RegenerateSceneEditorData();
	}
}


void Scene::Draw(DeviceResources* deviceRes) {
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		it->Draw(deviceRes,false);
	}
}


void Scene::Reset()
{

}

void Scene::SaveScene()
{
	std::ofstream fout;

	std::string path = "Scenes/";

	const size_t cSize = wcslen(currentId.c_str()) + 1;
	char* wc = new char[cSize];
	wcstombs(wc, currentId.c_str(), cSize);
	path.append(wc);
	delete[] wc;
	path.append(".scene");

	fout.open(path);

	if (!fout.fail()) {
		fout << "Writing this to a file.\n";
		
	}

	fout.close();
}

void Scene::LoadScene(std::wstring filepath)
{
}

void Scene::DeleteSceneFromDisk()
{
	if (mode == SCENE_EDITOR) {
		std::wstring path;
		path.append(L"Scenes/");
		path.append(currentId.c_str());
		path.append(L".scene");
		_wremove(path.c_str());
	}
}

const std::vector<Material>& Scene::GetMaterials()
{
	return materials;
}

Material* Scene::AddMaterial(std::wstring materialID, DeviceResources* deviceRes)
{
	Material* searchResult = GetMaterial(materialID);

	if (searchResult != nullptr) return searchResult;

	materials.push_back(Material(materialID));
	(materials.end() - 1)->Create(deviceRes);
	return &(*(materials.end() - 1));
}

Material* Scene::GetMaterial(std::wstring materialId)
{
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), materialId.c_str()) == 0) return &(*it);
	}

	return nullptr;
}

void Scene::Im()
{
	ImGui::Begin("Current Scene", NULL);
	ImGui::SetWindowCollapsed(false);
	ImGui::SetWindowSize(ImVec2(500, 600), ImGuiCond_Always);

	if (ImGui::CollapsingHeader("General")) {
		ImGui::InputText("Scene name", editorData->sceneId, 64);
		ImGui::SameLine();
		if (ImGui::Button("Rename")) {
			const size_t cSize = strlen(editorData->sceneId) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, editorData->sceneId, cSize);

			currentId.clear();
			currentId.append(wc);
			delete[] wc;
		}

		ImGui::InputText("Skybox texture", editorData->skyBox, 64);

		if (ImGui::DragFloat3("Light Direction", editorData->lightDirection, 1.0f, -360.f, 360.0f)) {
		}
	}



	if (ImGui::CollapsingHeader("Objects")) {

		std::map<GameEntity*, SceneEditorEntityData*>::iterator iter;
		for (iter = editorData->entities.begin(); iter != editorData->entities.end(); ++iter) {
			ImGui::PushID(iter->first);
			if (ImGui::CollapsingHeader("Entity")) {
				ImGui::InputText("ID", iter->second->id, 64);
				ImGui::SameLine();
				if (ImGui::Button("Rename")) {
					const size_t cSize = strlen(iter->second->id) + 1;
					wchar_t* wc = new wchar_t[cSize];
					mbstowcs(wc, iter->second->id, cSize);

					iter->first->SetID(wc);
					delete[] wc;
				}
				if (ImGui::DragFloat3("Position", iter->second->position, 1.0f, -150.f, 150.0f)) {
					iter->first->SetPosition(Vector3(iter->second->position));
				}

				if (ImGui::DragFloat3("Rotation", iter->second->rotation, 1.0f, -360.f, 360.0f)) {
					iter->first->SetRotation(Vector3(iter->second->rotation));
				}

				if (ImGui::DragFloat("Scale", &(iter->second->scale), 1.0f, 0.1f, 50)) {
					iter->first->SetScale(iter->second->scale);
				}

				if (ImGui::Checkbox("Interactable", &(iter->second->interactable))) {
					iter->first->SetInteractable(iter->second->interactable);
				}
			}
			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("Control")) {
		if(ImGui::Button("Save scene")){
			SaveScene();
		}
		ImGui::Spacing();
		if (ImGui::Button("Delete scene")) {
			DeleteSceneFromDisk();
		}
	}

	ImGui::End();
}

void Scene::RegenerateSceneEditorData()
{
	if (editorData) {
		std::map<GameEntity*, SceneEditorEntityData*>::iterator itr = editorData->entities.begin();
		if (itr != editorData->entities.end())
		{
			// found it - delete it
			delete itr->second;
			editorData->entities.erase(itr);
		}

		delete editorData;
	}


	editorData = new SceneEditorData;

	std::vector<Material>::iterator itMat;
	std::vector<GameModel>::iterator itModel;
	std::vector<GameEntity>::iterator itEntities;
	for (itMat = materials.begin(); itMat != materials.end(); ++itMat) {
		for (itModel = (*itMat).GetModels().begin(); itModel != (*itMat).GetModels().end(); ++itModel) {
			for (itEntities = (*itModel).GetEntities().begin(); itEntities != (*itModel).GetEntities().end(); ++itEntities) {

				const size_t cSize = wcslen((*itEntities).GetID().c_str()) + 1;

				wcstombs(editorData->entities[&(*itEntities)]->id, (*itEntities).GetID().c_str(), cSize);

				editorData->entities[&(*itEntities)] = new SceneEditorEntityData();
				editorData->entities[&(*itEntities)]->interactable = (*itEntities).IsInteractable();
				editorData->entities[&(*itEntities)]->scale = (*itEntities).GetScale();
				editorData->entities[&(*itEntities)]->position[0] = (*itEntities).GetPosition().x;
				editorData->entities[&(*itEntities)]->position[1] = (*itEntities).GetPosition().y;
				editorData->entities[&(*itEntities)]->position[2] = (*itEntities).GetPosition().z;
				editorData->entities[&(*itEntities)]->rotation[0] = (*itEntities).GetRotation().x;
				editorData->entities[&(*itEntities)]->rotation[1] = (*itEntities).GetRotation().y;
				editorData->entities[&(*itEntities)]->rotation[2] = (*itEntities).GetRotation().z;

			}
		}
	}
}
