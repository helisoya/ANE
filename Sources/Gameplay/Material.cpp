#include "pch.h"

#include "Material.h"
#include "Utils.h"





Material::Material(std::wstring texturePath) : id(texturePath), texture(texturePath)
{
}

void Material::Create(DeviceResources* deviceRes) {
	texture.Create(deviceRes);
}

void Material::Draw(DeviceResources* deviceRes, bool isInstanced) {
	texture.Apply(deviceRes);

	std::vector<GameModel>::iterator it;
	for (it = gameModels.begin(); it != gameModels.end(); ++it) {
		(*it).Draw(deviceRes, isInstanced);
	}
}

GameModel* Material::GetModel(std::wstring modelId)
{
	std::vector<GameModel>::iterator it;
	for (it = gameModels.begin(); it != gameModels.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), modelId.c_str()) == 0) return &(*it);
	}

	return nullptr;
}

std::vector<GameModel>& Material::GetModels()
{
	return gameModels;
}

GameModel* Material::AddModel(std::wstring modelID, DeviceResources* deviceRes)
{
	GameModel* searchResult = GetModel(modelID);

	if (searchResult != nullptr) return searchResult;


	std::wstring fullPath;
	fullPath.append(L"Models/Scenes/");
	fullPath.append(modelID.c_str());
	fullPath.append(L".obj");
	gameModels.push_back(GameModel(modelID,id, fullPath, deviceRes));
	return &(*(gameModels.end()-1));
}

bool Material::RemoveEntity(const std::wstring& modelId,const USHORT& entityId)
{
	std::vector<GameModel>::iterator it;
	for (it = gameModels.begin(); it != gameModels.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), modelId.c_str()) == 0) {
			if (it->RemoveEntity(entityId)) {
				gameModels.erase(it);
			
				if (gameModels.size() == 0) {
					texture.Release();
					return true;
				}
			}
			break;
		}
	}

	return false;
}

bool Material::RemoveModel(const std::wstring& modelId)
{
	std::vector<GameModel>::iterator it;
	for (it = gameModels.begin(); it != gameModels.end(); ++it) {
		if (wcscmp((*it).GetID().c_str(), modelId.c_str()) == 0) {
			it->Release();
			gameModels.erase(it);

			if (gameModels.size() == 0) {
				texture.Release();
				return true;
			}
			break;
		}
	}
	return false;
}

void Material::Release()
{
	std::vector<GameModel>::iterator it;
	for (it = gameModels.begin(); it != gameModels.end(); ++it) {
		it->Release();
	}
	gameModels.clear();
	texture.Release();
}

const std::wstring& Material::GetID() {
	return id;
}

