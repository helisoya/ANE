#pragma once

#include "ANEBuffer.h"
#include "ANETexture.h"
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class ANEModel
{
	struct VertexType
	{
		XMFLOAT4 position;
		XMFLOAT2 texture;
	};

public:
	ANEModel();
	ANEModel(const ANEModel& other);
	~ANEModel();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* texturePath);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	ID3D11ShaderResourceView* GetTexture();

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ANETexture* m_texture;
	ANEVertexBuffer<VertexType>* m_vertexBuffer;
	ANEIndexBuffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

