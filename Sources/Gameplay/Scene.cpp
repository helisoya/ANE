#include "pch.h"

#include "Engine/DefaultResources.h"
#include "Scene.h"
#include "Utils.h"
#include "iostream"
#include "fstream"
#include "sstream"
#include <string>
#include <filesystem>

Scene::Scene() : currentId(L"Test"), editorData(nullptr), skybox() {
}

Scene::~Scene() {
	if (editorData) {
		std::map<GameEntity*, SceneEditorEntityData*>::iterator itr = editorData->entities.begin();
		while (itr != editorData->entities.end())
		{
			// found it - delete it
			delete itr->second;
			++itr;
		}

		editorData->entities.clear();

		delete editorData;
	}
}

void Scene::Generate(DeviceResources* deviceRes, GameMode mode) {

	this->deviceRes = deviceRes;
	skybox.Generate(deviceRes);
	directionalLight.Generate(deviceRes);
	mode = mode;
}


void Scene::Draw(DeviceResources* deviceRes) {
	std::vector<Material>::iterator it;
	for (it = materials.begin(); it != materials.end(); ++it) {
		it->Draw(deviceRes,false);
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
}

void Scene::SaveScene()
{
	std::ofstream fout;

	std::string path = "Scenes/";

	char* ca = toChar(currentId.c_str());

	path.append(ca);
	delete[] ca;
	path.append(".scene");

	fout.open(path);

	if (!fout.fail()) {
		ca = toChar(skybox.GetTextureName().c_str());
		fout << "S " << ca << std::endl;
		delete[] ca;

		fout << "L " << directionalLight.GetDirection().x
			<< " " << directionalLight.GetDirection().y
			<< " " << directionalLight.GetDirection().z << std::endl;

		std::vector<Material>::iterator itMat;
		std::vector<GameModel>::iterator itModel;
		std::vector<GameEntity>::iterator itEntities;

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
						<< itEntities->GetRotation().z << " " << itEntities->GetScale() << " " 
						<< itEntities->IsInteractable() << std::endl;

				}
			}
		}
	}

	fout.close();
}

void Scene::LoadScene(std::wstring filepath)
{
	Reset();

	std::ifstream fin;
	char input, input2;

	char model[64];
	char material[64];
	char name[64];
	float f1, f2, f3;
	bool b1;

	wchar_t* wca;

	Material* mat;
	GameModel* md;
	GameEntity* et;

	std::wstring correctPath;
	correctPath.append(L"Scenes/");
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

			fin >> f1 >> b1;
			et->SetScale(f1);
			et->SetInteractable(b1);
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();
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
	crateModel->GetEntity(L"Obj1")->SetScale(2);
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

void Scene::RemoveEntity(const std::wstring& materialId, const std::wstring& modelId, const USHORT& id)
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

void Scene::Im()
{
	if (editorData == nullptr) {
		// Generate modification data
		RegenerateSceneEditorData();
	}

	wchar_t* wca = nullptr;
	char* ca = nullptr;

	// Global scene edition
	ImGui::Begin("Scenes", NULL);
	ImGui::SetWindowCollapsed(false);
	ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_Always);

	if (ImGui::CollapsingHeader("Scenes")) {
		for (int i = 0; i < editorData->scenes.size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Button(editorData->scenes.at(i).c_str())) {

				wca = toWChar(editorData->scenes.at(i).c_str());
				LoadScene(wca);
				delete[] wca;

				ImGui::PopID();
				ImGui::End();
				RegenerateSceneEditorData();
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

	// Current scene edition

	ImGui::Begin("Current Scene", NULL);
	ImGui::SetWindowCollapsed(false);
	ImGui::SetWindowSize(ImVec2(500, 600), ImGuiCond_Always);



	if (ImGui::CollapsingHeader("General")) {
		ImGui::Text("Id : ");
		ImGui::SameLine();
		ca = toChar(currentId.c_str());
		ImGui::Text(ca);
		delete[] ca;
		
		ImGui::InputText("Rename", editorData->sceneId, 64);
		ImGui::SameLine();
		if (ImGui::Button("Apply")) {
			wca = toWChar(editorData->sceneId);

			currentId.clear();
			currentId.append(wca);
			delete[] wca;
		}

		ImGui::InputText("Skybox texture", editorData->skyBox, 64);
		ImGui::SameLine();
		if (ImGui::Button("Change")) {
			wca = toWChar(editorData->skyBox);
			SetSkyboxTexture(wca);
			delete[] wca;
		}

		if (ImGui::DragFloat3("Light Direction", editorData->lightDirection, 0.1f, -1.0f, 1.0f)) {
			directionalLight.SetDirection(Vector3(editorData->lightDirection));
		}

		float x[2] = { 0,editorData->lightDirection[0] };
		float y[2] = { 0,editorData->lightDirection[1] };
		float z[2] = { 0,editorData->lightDirection[2] };

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
		ImGui::Text(std::to_string(editorData->entities.size()).c_str());

		std::map<GameEntity*, SceneEditorEntityData*>::iterator iter;
		for (iter = editorData->entities.begin(); iter != editorData->entities.end(); ++iter) {
			ImGui::PushID(iter->first);
			if (ImGui::CollapsingHeader("Entity")) {

				ImGui::Text("Material : ");
				ImGui::SameLine();
				ca = toChar(iter->first->GetMaterialId().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::Text("Model : ");
				ImGui::SameLine();
				ca = toChar(iter->first->GetModelId().c_str());
				ImGui::Text(ca);
				delete[] ca;

				ImGui::Text("Id : ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(iter->first->GetID()).c_str());

				ImGui::InputText("Name", iter->second->name, 64);
				ImGui::SameLine();
				if (ImGui::Button("Rename")) {
					wca = toWChar(iter->second->name);
					iter->first->SetName(wca);
					delete[] wca;
				}
				if (ImGui::DragFloat3("Position", iter->second->position, 1.0f, -150.f, 150.0f)) {
					iter->first->SetPosition(Vector3(iter->second->position));
				}

				if (ImGui::DragFloat3("Rotation", iter->second->rotation, 0.25f, -XM_2PI, XM_2PI)) {
					iter->first->SetRotation(Vector3(iter->second->rotation));
				}

				if (ImGui::DragFloat("Scale", &(iter->second->scale), 1.0f, 0.1f, 50)) {
					iter->first->SetScale(iter->second->scale);
				}

				if (ImGui::Checkbox("Interactable", &(iter->second->interactable))) {
					iter->first->SetInteractable(iter->second->interactable);
				}

				if (ImGui::Button("Remove")) {

					GetMaterial(iter->first->GetMaterialId())->RemoveEntity(iter->first->GetModelId(), iter->first->GetID());

					delete iter->second;
					editorData->entities.erase(iter);

					ImGui::PopID();
					ImGui::End();
					RegenerateSceneEditorData();
					return;
				}
			}
			ImGui::PopID();
		}
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
	if (editorData) {
		std::map<GameEntity*, SceneEditorEntityData*>::iterator itr = editorData->entities.begin();
		while (itr != editorData->entities.end())
		{
			// found it - delete it
			delete itr->second;
			++itr;
		}

		editorData->entities.clear();

		delete editorData;
	}


	editorData = new SceneEditorData;

	char* ca = toChar(currentId.c_str());
	strcpy(editorData->sceneId, ca);
	delete[] ca;

	ca = toChar(skybox.GetTextureName().c_str());
	strcpy(editorData->skyBox, ca);
	delete[] ca;

	std::string path = "Scenes/";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		editorData->scenes.push_back(entry.path().filename().replace_extension("").string());
	}

	editorData->lightDirection[0] = directionalLight.GetDirection().x;
	editorData->lightDirection[1] = directionalLight.GetDirection().y;
	editorData->lightDirection[2] = directionalLight.GetDirection().z;
		
	std::vector<Material>::iterator itMat;
	std::vector<GameModel>::iterator itModel;
	std::vector<GameEntity>::iterator itEntities;
	for (itMat = materials.begin(); itMat != materials.end(); ++itMat) {
		for (itModel = (*itMat).GetModels().begin(); itModel != (*itMat).GetModels().end(); ++itModel) {
			for (itEntities = (*itModel).GetEntities().begin(); itEntities != (*itModel).GetEntities().end(); ++itEntities) {

				const size_t cSize = wcslen((*itEntities).GetName().c_str()) + 1;

				wcstombs(editorData->entities[&(*itEntities)]->name, (*itEntities).GetName().c_str(), cSize);

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

