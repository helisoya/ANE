#include "pch.h"

#include "GameModel.h"
#include "Engine/DefaultResources.h"
#include "Utils.h"
#include <iostream>
#include <fstream>



void GameModel::CreateCube(DeviceResources* deviceRes)
{
	ib.Release();
	vb.Release();

	// Front
	PushTriangle(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(0.5, -0.5, -0.5)
		,Vector3::Forward, Vector3::Forward, Vector3::Forward,
		Vector2(0,0),Vector2(1,0),Vector2(0,1),false);
	PushTriangle(Vector3(0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(0.5, -0.5, -0.5)
		, Vector3::Forward, Vector3::Forward, Vector3::Forward,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), true);

	// Left
	PushTriangle(Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5)
		, Vector3::Left, Vector3::Left, Vector3::Left,
		Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), false);
	PushTriangle(Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5)
		, Vector3::Left, Vector3::Left, Vector3::Left,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), true);

	// Right
	PushTriangle(Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5), Vector3(0.5, -0.5, -0.5)
		, Vector3::Right, Vector3::Right, Vector3::Right,
		Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), true);
	PushTriangle(Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, 0.5), Vector3(0.5, -0.5, -0.5)
		, Vector3::Right, Vector3::Right, Vector3::Right,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), false);

	// Back
	PushTriangle(Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5)
		, Vector3::Backward, Vector3::Backward, Vector3::Backward,
		Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), true);
	PushTriangle(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5)
		, Vector3::Backward, Vector3::Backward, Vector3::Backward,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), false);

	// Up
	PushTriangle(Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, 0.5, -0.5)
		, Vector3::Up, Vector3::Up, Vector3::Up,
		Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), false);
	PushTriangle(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, 0.5, -0.5)
		, Vector3::Up, Vector3::Up, Vector3::Up,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), true);

	// Up
	PushTriangle(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, -0.5)
		, Vector3::Down, Vector3::Down, Vector3::Down,
		Vector2(0, 0), Vector2(1, 0), Vector2(0, 1), true);
	PushTriangle(Vector3(0.5, -0.5, 0.5), Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, -0.5)
		, Vector3::Down, Vector3::Down, Vector3::Down,
		Vector2(1, 1), Vector2(1, 0), Vector2(0, 1), false);

	vb.Create(deviceRes);
	ib.Create(deviceRes);
}

GameModel::GameModel(DeviceResources* deviceRes) : materialId(L"None")
{
	this->id = L"Cube";
	instbuffer.data = {};
	instbuffer.Create(deviceRes);
}

GameModel::GameModel(std::wstring &id, std::wstring materialId,std::wstring &filePath, DeviceResources* deviceRes) :
	materialId(materialId)
{
	this->id = id;

	instbuffer.data = {};
	instbuffer.Create(deviceRes);
	
	LoadFromOBJ(filePath, deviceRes);
}

void GameModel::PushTriangle(Vector3 a, Vector3 b, Vector3 c,
	Vector3 na, Vector3 nb, Vector3 nc,
	Vector2 uva, Vector2 uvb, Vector2 uvc, bool front)
{

	auto aIdx = vb.PushVertex({ ToVec4(a), ToVec4Normal(na), uva });
	auto bIdx = vb.PushVertex({ ToVec4(b), ToVec4Normal(nb), uvb });
	auto cIdx = vb.PushVertex({ ToVec4(c), ToVec4Normal(nc), uvc });

	if (front) {
		ib.PushTriangle(aIdx, bIdx, cIdx);
	}
	else {
		ib.PushTriangle(aIdx, cIdx, bIdx);
	}
}

void GameModel::LoadFromOBJ(std::wstring &filePath, DeviceResources* deviceRes)
{
	vb.Clear();
	ib.Clear();

	std::ifstream fin;
	char input, input2;

	float v1, v2, v3;
	int i1, i2, i3, i4, i5, i6, i7, i8, i9;

	std::vector<Vector3> vertexs;
	std::vector<Vector2> uvs;
	std::vector<Vector3> normals;

	fin.open(filePath);

	if (fin.fail())
	{
		return;
	}

	fin.get(input);
	while (!fin.eof()) {

		if (input == 'v') {
			fin.get(input);
			if (input == 't') {
				// UV
				fin >> v1 >> v2;
				v2 = 1.0f - v2;
				uvs.push_back(Vector2(v1, v2));
			}
			else if (input == 'n') {
				// Normal
				fin >> v1 >> v2 >> v3;
				v3 *= -1.0f;
				normals.push_back(Vector3(v1, v2, v3));
			}
			else {
				// Vertex
				fin >> v1 >> v2 >> v3;
				v3 *= -1.0f;
				vertexs.push_back(Vector3(v1, v2, v3));
			}
		}
		else if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> i1 >> input2 >> i2 >> input2 >> i3
					>> i4 >> input2 >> i5 >> input2 >> i6
					>> i7 >> input2 >> i8 >> input2 >> i9;

				PushTriangle(vertexs.at(i1 - 1), vertexs.at(i4 - 1), vertexs.at(i7 - 1), 
					normals.at(i3-1), normals.at(i6 - 1), normals.at(i9 - 1),
					uvs.at(i2-1), uvs.at(i5 - 1), uvs.at(i8 - 1));
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	vb.Create(deviceRes);
	ib.Create(deviceRes);
}


void GameModel::Draw(DeviceResources* deviceRes, bool allowInstancing) {
	auto gpuRes = DefaultResources::Get();
	std::vector<GameEntity>::iterator it;


	if (!allowInstancing || entities.size() < INSTANCING_THRESHOLD) {
		if (vb.Size() == 0) return;
		// If not instanced, draw normaly

		vb.Apply(deviceRes, 0);
		ib.Apply(deviceRes);

		for (it = entities.begin(); it != entities.end(); ++it) {
			gpuRes->cbModel.data.model = (*it).GetWorldMatrix().Transpose();
			gpuRes->cbModel.data.isInstance = false;
			gpuRes->cbModel.UpdateBuffer(deviceRes);

			deviceRes->GetD3DDeviceContext()->DrawIndexed(ib.Size(), 0, 0);
		}
	}
	else {
		// If instanced, add the instance buffer to the vertex buffer, then draw
		
		UINT strides[2] = { sizeof(VertexLayout_PositionNormalUV), sizeof(Vector4)*4 };
		UINT offsets[2] = { 0, 0 };

		ID3D11Buffer* vertInstBuffers[2] = { vb.get().Get(), instbuffer.get().Get() };

		gpuRes->cbModel.data.model = Matrix::Identity.Transpose();
		gpuRes->cbModel.data.isInstance = true;
		gpuRes->cbModel.UpdateBuffer(deviceRes);

		ib.Apply(deviceRes);
		//Set the models vertex buffer
		deviceRes->GetD3DDeviceContext()->IASetVertexBuffers(0, 2, vertInstBuffers, strides, offsets);
		deviceRes->GetD3DDeviceContext()->DrawIndexedInstanced(ib.Size(), entities.size(), 0, 0, 0);
		
	}


}

void GameModel::ResetInstanceBuffer(DeviceResources* deviceRes)
{
	instbuffer.Release();

	std::vector<GameEntity>::iterator it;
	Matrix transposed;
	for (it = entities.begin(); it != entities.end(); ++it) {
		transposed = (*it).GetWorldMatrix();
		instbuffer.data.push_back(Vector4(transposed.m[0]));
		instbuffer.data.push_back(Vector4(transposed.m[1]));
		instbuffer.data.push_back(Vector4(transposed.m[2]));
		instbuffer.data.push_back(Vector4(transposed.m[3]));
	}

	instbuffer.Create(deviceRes);
}

GameEntity* GameModel::AddEntity(std::wstring name)
{
	while (GetEntity(name) != nullptr) {
		name.append(L"I");
	}

	entities.push_back(GameEntity(name,this->id,materialId));
	return &(*(entities.end()-1));
}

GameEntity* GameModel::GetEntity(std::wstring name)
{
	std::vector<GameEntity>::iterator it;
	for (it = entities.begin(); it != entities.end(); ++it) {
		if (wcscmp(name.c_str(), (*it).GetName().c_str()) == 0) {
			return &(*it);
		}
	}
	return nullptr;
}


std::vector<GameEntity>& GameModel::GetEntities()
{
	return entities;
}

bool GameModel::RemoveEntity(const std::wstring& name)
{
	std::vector<GameEntity>::iterator it;
	for (it = entities.begin(); it != entities.end(); ++it) {
		if (wcscmp(name.c_str(), (*it).GetName().c_str()) == 0) {
			entities.erase(it);
			break;
		}
	}

	if (entities.size() == 0) {
		Release();

		return true;
	}

	return false;
}

void GameModel::RemoveAllEntities(bool releaseAfterwards)
{
	entities.clear();
	if (releaseAfterwards) Release();
}

void GameModel::Release() {
	entities.clear();
	ib.Release();
	vb.Release();
	instbuffer.Release();
}

const std::wstring& GameModel::GetID() {
	return id;
}