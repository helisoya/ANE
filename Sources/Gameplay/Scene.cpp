#include "pch.h"

#include "Engine/DefaultResources.h"
#include "Scene.h"
#include "Utils.h"
#include "iostream"
#include "fstream"
#include "sstream"
#include <string>
#include <filesystem>

Scene::Scene() : currentId(L"Test"), editorData(nullptr), skybox(), deviceRes(nullptr), mode(GAME) {
}

Scene::~Scene() {
	if (editorData) {
		ClearCurrentSceneEditorDataContent();
		editorData->interactionsModel->Release();
		editorData->interactionsTexture->Release();
		delete editorData->interactionsTexture;
		delete editorData->interactionsModel;
		delete editorData;
	}
}

void Scene::Generate(DeviceResources* deviceRes, GameMode mode) {

	this->deviceRes = deviceRes;
	skybox.Generate(deviceRes);
	directionalLight.Generate(deviceRes);
	this->mode = mode;

	if (mode == SCENE_EDITOR) {
		editorData = new SceneEditorData();
		editorData->interactionsTexture = new Texture(L"Interaction");
		editorData->interactionsTexture->Create(deviceRes);
		editorData->interactionsModel = new GameModel(deviceRes);
		editorData->interactionsModel->CreateCube(deviceRes);
	}
}


void Scene::Draw(DeviceResources* deviceRes) {

	bool allowInstancing = (mode != SCENE_EDITOR || editorData->allowInstancing);

	if (mode != SCENE_EDITOR || editorData->drawEntities) {
		std::vector<Material>::iterator it;
		for (it = materials.begin(); it != materials.end(); ++it) {
			it->Draw(deviceRes, allowInstancing);
		}
	}
	if (mode == SCENE_EDITOR && editorData->drawInteractions) {
		editorData->interactionsTexture->Apply(deviceRes);
		editorData->interactionsModel->Draw(deviceRes, allowInstancing);
	}
}

void Scene::DrawSkybox(DeviceResources* deviceRes)
{
	skybox.Draw(deviceRes);
}

void Scene::ApplySkybox(DeviceResources* deviceRes)
{
	skybox.Apply(deviceRes);
}

void Scene::ApplyDirectionalLight(DeviceResources* deviceRes)
{
	directionalLight.Apply(deviceRes);
}


void Scene::ChangeLightDirection(Vector3 newDirection)
{
	directionalLight.SetDirection(newDirection);
}

Light& Scene::GetLight()
{
	return directionalLight;
}

void Scene::SetSkyboxTexture(std::wstring newTexture)
{
	skybox.ChangeTexture(newTexture,deviceRes);
}

void Scene::Reset()
{
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		it->Release();
	}
	materials.clear();

	interactions.clear();
}

void Scene::SaveScene()
{
	std::ofstream fout;

	std::string path = "Resources/Scenes/";

	char* ca = toChar(currentId.c_str());

	path.append(ca);
	delete[] ca;
	path.append(".scene");

	fout.open(path);

	if (!fout.fail()) {
		ca = toChar(skybox.GetTextureName().c_str());
		fout << "S " << ca << " \n";
		delete[] ca;

		fout << "L " << directionalLight.GetDirection().x
			<< " " << directionalLight.GetDirection().y
			<< " " << directionalLight.GetDirection().z << " \n";

		std::vector<Material>::iterator itMat;
		std::vector<GameModel>::iterator itModel;
		std::vector<GameEntity>::iterator itEntities;
		std::vector<Interaction>::iterator itInteraction;

		for (itMat = materials.begin(); itMat != materials.end(); ++itMat) {
			for (itModel = (*itMat).GetModels().begin(); itModel != (*itMat).GetModels().end(); ++itModel) {
				for (itEntities = (*itModel).GetEntities().begin(); itEntities != (*itModel).GetEntities().end(); ++itEntities) {
					
					ca = toChar(itMat->GetID().c_str());
					fout << "E " << ca << " ";
					delete[] ca;

					ca = toChar(itModel->GetID().c_str());
					fout << ca << " ";
					delete[] ca;

					ca = toChar(itEntities->GetName().c_str());
					fout << ca << " ";
					delete[] ca;
					
					fout << itEntities->GetPosition().x << " " 
						<< itEntities->GetPosition().y << " " << itEntities->GetPosition().z << " " 
						<< itEntities->GetRotation().x << " " << itEntities->GetRotation().y << " "
						<< itEntities->GetRotation().z << " " << itEntities->GetScale().x << " "
						<< itEntities->GetScale().y << " " << itEntities->GetScale().z << " \n";

				}
			}
		}

		for (itInteraction = interactions.begin(); itInteraction != interactions.end(); ++itInteraction) {

			ca = toChar(itInteraction->GetName().c_str());
			fout << "I " << ca << " ";
			delete[] ca;

			fout << (itInteraction->IsLinkedToEntity() ? 1 : 0) << " ";

			if (itInteraction->IsLinkedToEntity()) {
				ca = toChar(itInteraction->GetLinkedMaterialId().c_str());
				fout << ca << " ";
				delete[] ca;

				ca = toChar(itInteraction->GetLinkedModelId().c_str());
				fout << ca << " ";
				delete[] ca;

				ca = toChar(itInteraction->GetLinkedEntityID().c_str());
				fout << ca << " ";
				delete[] ca;
			}
			else {
				fout << "_ _ _ ";
			}


			fout << (itInteraction->IsInteractable() ? 1 : 0) << " ";
			

			fout << itInteraction->GetPosition().x << " " << itInteraction->GetPosition().y << " " 
				<< itInteraction->GetPosition().z <<  " "  << itInteraction->GetScale().x << " " 
				<< itInteraction->GetScale().y << " " << itInteraction->GetScale().z << " \n";

		}
	}

	fout.close();
}

void Scene::LoadScene(std::wstring filepath)
{
	Reset();

	currentId = filepath;

	std::ifstream fin;
	char input, input2;

	char model[64];
	char material[64];
	char name[64];
	float f1, f2, f3;
	bool b1;

	wchar_t* wca;
	wchar_t* wca2;
	wchar_t* wca3;

	Material* mat;
	GameModel* md;
	GameEntity* et;
	Interaction* interaction;

	std::wstring correctPath;
	correctPath.append(L"Resources/Scenes/");
	correctPath.append(filepath);
	correctPath.append(L".scene");

	fin.open(correctPath);

	if (fin.fail())
	{
		return;
	}

	fin.get(input);
	while (!fin.eof()) {

		if (input == 'L')
		{
			fin >> f1 >> f2 >> f3;
			ChangeLightDirection(Vector3(f1, f2, f3));
		}
		else if (input == 'S') {
			fin >> name;
			wca = toWChar(name);
			SetSkyboxTexture(wca);
			delete[] wca;
		}
		else if (input == 'E') {
			fin >> material >> model >> name;

			wca = toWChar(material);
			mat = AddMaterial(wca,deviceRes);
			delete[] wca;

			wca = toWChar(model);
			md = mat->AddModel(wca, deviceRes);
			delete[] wca;

			wca = toWChar(name);
			et = md->AddEntity(wca);
			delete[] wca;

			fin >> f1 >> f2 >> f3;
			et->SetPosition(Vector3(f1, f2, f3));
			fin >> f1 >> f2 >> f3;
			et->SetRotation(Vector3(f1, f2, f3));
			fin >> f1 >> f2 >> f3;
			et->SetScale(Vector3(f1, f2, f3));
		}
		else if (input == 'I') {
			fin >> name;

			wca = toWChar(name);
			interaction = AddInteraction(wca);
			delete[] wca;

			fin >> b1 >> material >> model >> name;

			wca = toWChar(material);
			wca2 = toWChar(model);
			wca3 = toWChar(name);
			interaction->SetLinkToEntity(b1, wca, wca2, wca3);
			delete[] wca;
			delete[] wca2;
			delete[] wca3;

			fin >> b1;
			interaction->SetIsInteractable(b1);

			fin >> f1 >> f2 >> f3;
			interaction->SetPosition(Vector3(f1, f2, f3));

			fin >> f1 >> f2 >> f3;
			interaction->SetScale(Vector3(f1, f2, f3));
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	RefreshInstanceBuffers();

	if (mode == SCENE_EDITOR) {
		RegenerateSceneEditorData();
	}
}

void Scene::DeleteSceneFromDisk()
{
	if (mode == SCENE_EDITOR) {
		std::wstring path;
		path.append(L"Resources/Scenes/");
		path.append(currentId.c_str());
		path.append(L".scene");
		if (std::filesystem::exists(path)) {
			std::filesystem::remove(path);
		}
	}
}

void Scene::LoadDebugScene()
{
	Reset();

	currentId.clear();
	currentId.append(L"NewScene");
	SetSkyboxTexture(L"skybox");
	ChangeLightDirection(Vector3(0, 1, 0.5));

	Material* material = AddMaterial(L"Default", deviceRes);
	GameModel* crateModel = material->AddModel(L"Crate", deviceRes);
	crateModel->AddEntity(L"Obj1");
	crateModel->AddEntity(L"Obj2")->SetPosition(Vector3(5, 0, 0));
	crateModel->GetEntity(L"Obj1")->SetScale(Vector3(2,2,2));

	AddInteraction(L"Exit")->SetScale(Vector3(10,5,2));
	GetInteraction(L"Exit")->SetPosition(Vector3(0, 0, 7));

	RefreshInstanceBuffers();

	if (mode == SCENE_EDITOR) {
		RegenerateSceneEditorData();
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

Interaction* Scene::AddInteraction(std::wstring name)
{
	Interaction* searchResult = GetInteraction(name);

	if (searchResult != nullptr) return searchResult;

	interactions.push_back(Interaction(name));
	return &(*(interactions.end() - 1));
}

Interaction* Scene::GetInteraction(std::wstring name)
{
	std::vector<Interaction>::iterator it;
	for (it = interactions.begin(); it != interactions.end(); ++it) {
		if (wcscmp((*it).GetName().c_str(), name.c_str()) == 0) return &(*it);
	}

	return nullptr;
}

void Scene::RemoveInteraction(const std::wstring& name)
{
	std::vector<Interaction>::iterator it;
	for (it = interactions.begin(); it != interactions.end(); ++it) {
		if (wcscmp((*it).GetName().c_str(), name.c_str()) == 0){
			interactions.erase(it);
			break;
		}
		
	}
}

void Scene::RemoveEntity(const std::wstring& materialId, const std::wstring& modelId, const std::wstring& id)
{
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), materialId.c_str()) == 0) {
			if (it->RemoveEntity(modelId, id)) {
				materials.erase(it);
			}
			break;
		}
	}
}

void Scene::RemoveModel(const std::wstring& materialId, const std::wstring& modelId)
{
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), materialId.c_str()) == 0) {
			if (it->RemoveModel(modelId)) {
				materials.erase(it);
			}
			break;
		}
	}
}

void Scene::RemoveMaterial(const std::wstring& materialId)
{
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), materialId.c_str()) == 0) {
			materials.erase(it);
			materials.erase(it);
			break;
		}
	}
}

void Scene::RefreshInstanceBuffers()
{
	if (mode == SCENE_EDITOR) {
		editorData->interactionsModel->ResetInstanceBuffer(deviceRes);
	}

	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		it->RegenerateModelsInstanceBuffers(deviceRes);
	}
}

void Scene::Im()
{

	wchar_t* wca = nullptr;
	wchar_t* wca2 = nullptr;
	wchar_t* wca3 = nullptr;
	char* ca = nullptr;

#pragma region Scenes

	// Global scene edition
	ImGui::Begin("Scenes", NULL);
	ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_Always);

	ImGui::Checkbox("Allow Instancing (Rendering)", &editorData->allowInstancing);

	if (ImGui::CollapsingHeader("Scenes")) {
		for (int i = 0; i < editorData->scenes.size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Button(editorData->scenes.at(i).c_str())) {

				wca = toWChar(editorData->scenes.at(i).c_str());
				LoadScene(wca);
				delete[] wca;

				ImGui::PopID();
				ImGui::End();
				return;
			}
			ImGui::PopID();
		}

		ImGui::Spacing();

		if(ImGui::Button("New scene")) {
			LoadDebugScene();
			RegenerateSceneEditorData();
			ImGui::End();
			return;
		}
	}

	if (ImGui::CollapsingHeader("New Object")) {
		ImGui::InputText("Material", editorData->newObjectMaterial, 64);
		ImGui::InputText("Model", editorData->newObjectModel, 64);

		if (ImGui::Button("Add")) {

			wca = toWChar(editorData->newObjectMaterial);
			Material* material = AddMaterial(wca, deviceRes);
			delete[] wca;
			wca = toWChar(editorData->newObjectModel);
			material->AddModel(wca, deviceRes)->AddEntity(L"NewEntity");
			material->RegenerateModelsInstanceBuffers(deviceRes);
			delete[] wca;

			ImGui::End();

			char tempMat[64];
			char tempModel[64];

			strcpy(tempMat, editorData->newObjectMaterial);
			strcpy(tempModel, editorData->newObjectModel);

			RegenerateSceneEditorData();

			strcpy(editorData->newObjectMaterial, tempMat);
			strcpy(editorData->newObjectModel, tempModel);

			return;
		}
	}

	ImGui::End();

#pragma endregion

#pragma region CurrentScene
	// Current scene edition

	ImGui::Begin("Current Scene",NULL);
	ImGui::SetWindowSize(ImVec2(500, 600), ImGuiCond_Always);

	std::map<Interaction*, SceneEditorInteractionData*>::iterator iterInteraction;
	std::map<GameEntity*, SceneEditorEntityData*>::iterator iterEntity;

	if (ImGui::CollapsingHeader("General")) {
		ImGui::Text("Id : ");
		ImGui::SameLine();
		ca = toChar(currentId.c_str());
		ImGui::Text(ca);
		delete[] ca;
		
		ImGui::InputText("Rename", editorData->content->sceneId, 64);
		ImGui::SameLine();
		if (ImGui::Button("Apply")) {
			wca = toWChar(editorData->content->sceneId);

			currentId.clear();
			currentId.append(wca);
			delete[] wca;
		}

		ImGui::InputText("Skybox texture", editorData->content->skyBox, 64);
		ImGui::SameLine();
		if (ImGui::Button("Change")) {
			wca = toWChar(editorData->content->skyBox);
			SetSkyboxTexture(wca);
			delete[] wca;
		}

		if (ImGui::DragFloat3("Light Direction", editorData->content->lightDirection, 0.1f, -1.0f, 1.0f)) {
			directionalLight.SetDirection(Vector3(editorData->content->lightDirection));
		}

		float x[2] = { 0,editorData->content->lightDirection[0] };
		float y[2] = { 0,editorData->content->lightDirection[1] };
		float z[2] = { 0,editorData->content->lightDirection[2] };

		float xu[2] = { 0,0 };
		float yu[2] = { 0,-1 };
		float zu[2] = { 0,0 };

		float xf[2] = { 0,0 };
		float yf[2] = { 0,0 };
		float zf[2] = { 0,1 };

		float xr[2] = { 0,-1 };
		float yr[2] = { 0,0 };
		float zr[2] = { 0,0 };
		ImPlot3D::BeginPlot("Light Visualisation",ImVec2(400, 200), ImPlot3DFlags_NoPan | ImPlot3DFlags_NoZoom);
		ImPlot3D::SetupAxesLimits(-1, 1, -1, 1, -1, 1);
		ImPlot3D::PlotLine("Up", xu, yu, zu, 2);
		ImPlot3D::PlotLine("Front", xf, yf, zf, 2);
		ImPlot3D::PlotLine("Right", xr, yr, zr, 2);
		ImPlot3D::PlotLine("Direction", x, y, z, 2);
		ImPlot3D::EndPlot();
	}



	if (ImGui::CollapsingHeader("Objects")) {
		ImGui::Text("Entities count : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(editorData->content->entities.size()).c_str());
		ImGui::Checkbox("Draw Entities", &editorData->drawEntities);

		
		for (iterEntity = editorData->content->entities.begin(); iterEntity != editorData->content->entities.end(); ++iterEntity) {
			ImGui::PushID(iterEntity->first);
			if (ImGui::CollapsingHeader("Entity")) {

				ImGui::Text("Material : ");
				ImGui::SameLine();
				ca = toChar(iterEntity->first->GetMaterialId().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::Text("Model : ");
				ImGui::SameLine();
				ca = toChar(iterEntity->first->GetModelId().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::Text("Name : ");
				ImGui::SameLine();
				ca = toChar(iterEntity->first->GetName().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::InputText("Name", iterEntity->second->name, 64);
				ImGui::SameLine();
				if (ImGui::Button("Rename")) {
					wca = toWChar(iterEntity->second->name);
					
					if (strlen(iterEntity->second->name) > 0 && !GetMaterial(iterEntity->first->GetMaterialId())->GetModel(iterEntity->first->GetModelId())->GetEntity(wca)) {
						iterEntity->first->SetName(wca);
					}
					else {
						ImGui::OpenPopup("ErrorRenameEntity");
					}
					delete[] wca;
				}
				if (ImGui::DragFloat3("Position", iterEntity->second->position, 1.0f, -150.f, 150.0f)) {
					iterEntity->first->SetPosition(Vector3(iterEntity->second->position));
					RefreshInstanceBuffers();
				}

				if (ImGui::DragFloat3("Rotation", iterEntity->second->rotation, 0.25f, -XM_2PI, XM_2PI)) {
					iterEntity->first->SetRotation(Vector3(iterEntity->second->rotation));
					RefreshInstanceBuffers();
				}

				if (ImGui::DragFloat3("Scale", iterEntity->second->scale, 1.0f, 0.1f, 50)) {
					iterEntity->first->SetScale(Vector3(iterEntity->second->scale));
					RefreshInstanceBuffers();
				}

				if (ImGui::Button("Remove")) {

					// Check for orphan interactions

					for (iterInteraction = editorData->content->interactions.begin(); iterInteraction != editorData->content->interactions.end(); ++iterInteraction) {
						if (wcscmp((*iterInteraction).first->GetLinkedEntityID().c_str(), iterEntity->first->GetName().c_str()) == 0 &&
							wcscmp((*iterInteraction).first->GetLinkedMaterialId().c_str(), iterEntity->first->GetMaterialId().c_str()) == 0 &&
							wcscmp((*iterInteraction).first->GetLinkedModelId().c_str(), iterEntity->first->GetModelId().c_str()) == 0) {
							iterInteraction->first->SetLinkToEntity(false, L"_", L"_", L"_");
						}
					}

					GetMaterial(iterEntity->first->GetMaterialId())->RemoveEntity(iterEntity->first->GetModelId(), iterEntity->first->GetName());

					delete iterEntity->second;
					editorData->content->entities.erase(iterEntity);



					ImGui::PopID();
					ImGui::End();
					RefreshInstanceBuffers();
					RegenerateSceneEditorData();
					return;
				}
			}

			if (ImGui::BeginPopupModal("ErrorRenameEntity", NULL)) {
				ImGui::Text("Entities must have distinct non empty names");
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}


			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("Interactions")) {
		ImGui::Text("Interactions count : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(editorData->content->interactions.size()).c_str());
		ImGui::Checkbox("Draw Interactions", &editorData->drawInteractions);

		ImGui::InputText("Name", editorData->newInteraction, 64);
		ImGui::SameLine();
		if (ImGui::Button("Add New")) {
			wca = toWChar(editorData->newInteraction);

			if (strlen(editorData->newInteraction) > 0 && !GetInteraction(wca)) {
				AddInteraction(wca);
				RefreshInstanceBuffers();
				RegenerateSceneEditorData();
				ImGui::End();
				return;
			}
			else {
				ImGui::OpenPopup("ErrorCreateInteraction");
			}
			delete[] wca;
		}

		
		for (iterInteraction = editorData->content->interactions.begin(); iterInteraction != editorData->content->interactions.end(); ++iterInteraction) {
			ImGui::PushID(iterInteraction->first);
			if (ImGui::CollapsingHeader("Interaction")) {

				ImGui::Text("Name : ");
				ImGui::SameLine();
				ca = toChar(iterInteraction->first->GetName().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::InputText("Name", iterInteraction->second->name, 64);
				ImGui::SameLine();
				if (ImGui::Button("Rename")) {
					wca = toWChar(iterInteraction->second->name);

					if (strlen(iterInteraction->second->name) > 0 && !GetInteraction(wca)) {
						iterInteraction->first->SetName(wca);
					}
					else {
						ImGui::OpenPopup("ErrorRenameInteraction");
					}
					delete[] wca;
				}

				if (ImGui::Checkbox("Is linked to an entity", &iterInteraction->second->isLinkedToEntity)) {
					if (iterInteraction->second->isLinkedToEntity &&
						wcscmp(iterInteraction->first->GetLinkedMaterialId().c_str(), L"_") == 0 ||
						wcscmp(iterInteraction->first->GetLinkedEntityID().c_str(), L"_") == 0 ||
						wcscmp(iterInteraction->first->GetLinkedModelId().c_str(), L"_") == 0) {
						iterInteraction->second->isLinkedToEntity = false;
						ImGui::OpenPopup("LinkingInteraction");
					}
					else if (!iterInteraction->second->isLinkedToEntity) {
						iterInteraction->first->SetLinkToEntity(false, L"_", L"_", L"_");
					}
				}

				// Show Link data only if linking is active
				if (iterInteraction->second->isLinkedToEntity) {
					ImGui::Text("Material : ");
					ImGui::SameLine();
					ca = toChar(iterInteraction->first->GetLinkedMaterialId().c_str());
					ImGui::Text(ca);
					delete[] ca;

					ImGui::Text("Model : ");
					ImGui::SameLine();
					ca = toChar(iterInteraction->first->GetLinkedModelId().c_str());
					ImGui::Text(ca);
					delete[] ca;

					ImGui::Text("Entity : ");
					ImGui::SameLine();
					ca = toChar(iterInteraction->first->GetLinkedEntityID().c_str());
					ImGui::Text(ca);
					delete[] ca;

					if (ImGui::Button("Change Link")) {
						ImGui::OpenPopup("LinkingInteraction");
					}
				}


				if (ImGui::DragFloat3("Position", iterInteraction->second->position, 1.0f, -150.f, 150.0f)) {
					iterInteraction->first->SetPosition(Vector3(iterInteraction->second->position));
					iterInteraction->second->previewEntity->SetPosition(Vector3(iterInteraction->second->position));
					RefreshInstanceBuffers();
				}

				if (ImGui::DragFloat3("Scale", iterInteraction->second->scale, 1.0f, 0.1f, 50)) {
					iterInteraction->first->SetScale(Vector3(iterInteraction->second->scale));
					iterInteraction->second->previewEntity->SetScale(Vector3(iterInteraction->second->scale));
					RefreshInstanceBuffers();
				}

				if (ImGui::Checkbox("Interactable", &iterInteraction->second->interactable)) {
					iterInteraction->first->SetIsInteractable(iterInteraction->second->interactable);
				}


				if (ImGui::Button("Remove")) {
					RemoveInteraction(iterInteraction->first->GetName());

					delete iterInteraction->second;
					editorData->content->interactions.erase(iterInteraction);

					ImGui::PopID();
					ImGui::End();
					RefreshInstanceBuffers();
					RegenerateSceneEditorData();
					return;
				}
			}

			if (ImGui::BeginPopupModal("ErrorRenameInteraction", NULL)) {
				ImGui::Text("Interactions must have distinct non empty names");
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("LinkingInteraction",NULL)) {
				ImGui::Text("Specify the entity to link");
				ImGui::InputText("Material", iterInteraction->second->materialLinked, 64);
				ImGui::InputText("Model", iterInteraction->second->modelLinked, 64);
				ImGui::InputText("Entity Name", iterInteraction->second->entityLinked, 64);
				if (ImGui::Button("Apply")) {
					bool good = false;

					if (strlen(iterInteraction->second->materialLinked) > 0 &&
						strlen(iterInteraction->second->modelLinked) > 0 &&
						strlen(iterInteraction->second->entityLinked) > 0) {
						wca = toWChar(iterInteraction->second->materialLinked);
						wca2 = toWChar(iterInteraction->second->modelLinked);
						wca3 = toWChar(iterInteraction->second->entityLinked);

						if(GetMaterial(wca) && GetMaterial(wca)->GetModel(wca2) && 
							GetMaterial(wca)->GetModel(wca2)->GetEntity(wca3)) {
							iterInteraction->first->SetLinkToEntity(true, wca, wca2, wca3);
							iterInteraction->second->isLinkedToEntity = true;
							good = true;
						}

						delete[] wca;
						delete[] wca2;
						delete[] wca3;
					}

					if (!good) {
						ImGui::OpenPopup("ErrorLinkingInteraction",ImGuiPopupFlags_AnyPopupLevel);
					}
					else {
						ImGui::CloseCurrentPopup();
					}

				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("ErrorLinkingInteraction", NULL)) {
				ImGui::Text("The element you are trying to link doesn't exist");
				ImGui::Text("Please check the link values");
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::PopID();
		}

		if (ImGui::BeginPopupModal("ErrorCreateInteraction", NULL)) {
			ImGui::Text("Interactions must have distinct non empty names");
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	if (ImGui::CollapsingHeader("Markers")) {
		ImGui::Text("Markers count : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(0).c_str());
		ImGui::Checkbox("Draw Markers", &editorData->drawMarkers);
	}

	if (ImGui::CollapsingHeader("Control")) {
		if(ImGui::Button("Save scene")){
			SaveScene();
			RegenerateSceneEditorData();
			ImGui::End();
			return;
		}
		ImGui::Spacing();
		if (ImGui::Button("Reset Scene")) {
			Reset();
			RegenerateSceneEditorData();
			ImGui::End();
			return;
		}
		ImGui::Spacing();
		if (ImGui::Button("Delete scene")) {
			DeleteSceneFromDisk();
			RegenerateSceneEditorData();
			ImGui::End();
			return;
		}
	}

	ImGui::End();

}

void Scene::RegenerateSceneEditorData()
{
	ClearCurrentSceneEditorDataContent();

	/// Global data
	editorData->scenes.clear();
	std::string path = "Resources/Scenes/";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		editorData->scenes.push_back(entry.path().filename().replace_extension("").string());
	}

	// Scene Data
	editorData->content = new SceneEditorContentData;

	char* ca = toChar(currentId.c_str());
	strcpy(editorData->content->sceneId, ca);
	delete[] ca;

	ca = toChar(skybox.GetTextureName().c_str());
	strcpy(editorData->content->skyBox, ca);
	delete[] ca;

	editorData->content->lightDirection[0] = directionalLight.GetDirection().x;
	editorData->content->lightDirection[1] = directionalLight.GetDirection().y;
	editorData->content->lightDirection[2] = directionalLight.GetDirection().z;

	editorData->interactionsModel->RemoveAllEntities(false);
	std::vector<Interaction>::iterator itInter;
	for (itInter = interactions.begin(); itInter != interactions.end(); ++itInter) {
		editorData->content->interactions[&(*itInter)] = new SceneEditorInteractionData();
		
		ca = toChar((*itInter).GetName().c_str());
		strcpy(editorData->content->interactions[&(*itInter)]->name, ca);
		delete[] ca;

		editorData->content->interactions[&(*itInter)]->isLinkedToEntity = (*itInter).IsLinkedToEntity();
		ca = toChar((*itInter).GetLinkedEntityID().c_str());
		strcpy(editorData->content->interactions[&(*itInter)]->entityLinked, ca);
		delete[] ca;
		ca = toChar((*itInter).GetLinkedModelId().c_str());
		strcpy(editorData->content->interactions[&(*itInter)]->modelLinked, ca);
		delete[] ca;
		ca = toChar((*itInter).GetLinkedMaterialId().c_str());
		strcpy(editorData->content->interactions[&(*itInter)]->materialLinked, ca);
		delete[] ca;

		editorData->content->interactions[&(*itInter)]->scale[0] = (*itInter).GetScale().x;
		editorData->content->interactions[&(*itInter)]->scale[1] = (*itInter).GetScale().y;
		editorData->content->interactions[&(*itInter)]->scale[2] = (*itInter).GetScale().z;
		editorData->content->interactions[&(*itInter)]->position[0] = (*itInter).GetPosition().x;
		editorData->content->interactions[&(*itInter)]->position[1] = (*itInter).GetPosition().y;
		editorData->content->interactions[&(*itInter)]->position[2] = (*itInter).GetPosition().z;

		editorData->content->interactions[&(*itInter)]->interactable = (*itInter).IsInteractable();

		editorData->content->interactions[&(*itInter)]->previewEntity = editorData->interactionsModel->AddEntity((*itInter).GetName());
		editorData->content->interactions[&(*itInter)]->previewEntity->SetPosition((*itInter).GetPosition());
		editorData->content->interactions[&(*itInter)]->previewEntity->SetScale((*itInter).GetScale());
	}

		
	std::vector<Material>::iterator itMat;
	std::vector<GameModel>::iterator itModel;
	std::vector<GameEntity>::iterator itEntities;
	for (itMat = materials.begin(); itMat != materials.end(); ++itMat) {
		for (itModel = (*itMat).GetModels().begin(); itModel != (*itMat).GetModels().end(); ++itModel) {
			for (itEntities = (*itModel).GetEntities().begin(); itEntities != (*itModel).GetEntities().end(); ++itEntities) {
				editorData->content->entities[&(*itEntities)] = new SceneEditorEntityData();

				ca = toChar((*itEntities).GetName().c_str());
				strcpy(editorData->content->entities[&(*itEntities)]->name, ca);
				delete[] ca;
				
				editorData->content->entities[&(*itEntities)]->scale[0] = (*itEntities).GetScale().x;
				editorData->content->entities[&(*itEntities)]->scale[1] = (*itEntities).GetScale().y;
				editorData->content->entities[&(*itEntities)]->scale[2] = (*itEntities).GetScale().z;
				editorData->content->entities[&(*itEntities)]->position[0] = (*itEntities).GetPosition().x;
				editorData->content->entities[&(*itEntities)]->position[1] = (*itEntities).GetPosition().y;
				editorData->content->entities[&(*itEntities)]->position[2] = (*itEntities).GetPosition().z;
				editorData->content->entities[&(*itEntities)]->rotation[0] = (*itEntities).GetRotation().x;
				editorData->content->entities[&(*itEntities)]->rotation[1] = (*itEntities).GetRotation().y;
				editorData->content->entities[&(*itEntities)]->rotation[2] = (*itEntities).GetRotation().z;

			}
		}
	}
#pragma endregion
}

void Scene::ClearCurrentSceneEditorDataContent()
{
	if (editorData && editorData->content) {
		std::map<GameEntity*, SceneEditorEntityData*>::iterator itr = editorData->content->entities.begin();
		while (itr != editorData->content->entities.end())
		{
			// found it - delete it
			delete itr->second;
			++itr;
		}
		editorData->content->entities.clear();

		std::map<Interaction*, SceneEditorInteractionData*>::iterator itr2 = editorData->content->interactions.begin();
		while (itr2 != editorData->content->interactions.end())
		{
			// found it - delete it
			delete itr2->second;
			++itr2;
		}
		editorData->content->interactions.clear();

		delete editorData->content;
	}
}

