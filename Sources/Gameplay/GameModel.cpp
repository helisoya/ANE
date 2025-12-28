#include "pch.h"

#include "GameModel.h"
#include "Utils.h"
#include <iostream>
#include <fstream>


void GameModel::PushTriangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal,
	Vector2 uva, Vector2 uvb, Vector2 uvc, bool front)
{

	auto aIdx = vb.PushVertex({ ToVec4(a), ToVec4Normal(normal), uva });
	auto bIdx = vb.PushVertex({ ToVec4(b), ToVec4Normal(normal), uvb });
	auto cIdx = vb.PushVertex({ ToVec4(c), ToVec4Normal(normal), uvc });

	if (front) {
		ib.PushTriangle(aIdx, bIdx, cIdx);
	}
	else {
		ib.PushTriangle(aIdx, cIdx, bIdx);
	}
}

void GameModel::LoadFromOBJ(std::wstring filePath, DeviceResources* deviceRes)
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

				PushTriangle(vertexs.at(i1 - 1), vertexs.at(i4 - 1), vertexs.at(i7 - 1), normals.at(i3-1),
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


void GameModel::Draw(DeviceResources* deviceRes, bool isInstanced) {


	if (!isInstanced) {
		// If not instanced, draw normaly
		if (vb.Size() == 0) return;
		vb.Apply(deviceRes, 0);
		ib.Apply(deviceRes);
		deviceRes->GetD3DDeviceContext()->DrawIndexed(ib.Size(), 0, 0);
	}
	else {
		// If instanced, add the instance buffer to the vertex buffer, then draw
		UINT strides[2] = { sizeof(VertexLayout_PositionNormalUV), sizeof(Vector3) };
		UINT offsets[2] = { 0, 0 };

		ID3D11Buffer* vertInstBuffers[2] = { vb.get().Get(), instbuffer.get().Get() };

		ib.Apply(deviceRes);
		//Set the models vertex buffer
		deviceRes->GetD3DDeviceContext()->IASetVertexBuffers(0, 2, vertInstBuffers, strides, offsets);
		deviceRes->GetD3DDeviceContext()->DrawIndexedInstanced(ib.Size(), instbuffer.Size(), 0, 0, 0);
	}


}

void GameModel::ResetInstanceBuffer(DeviceResources* deviceRes, std::vector<Vector3>* positions)
{
	instbuffer.data = *positions;
	instbuffer.Create(deviceRes);
}
