#include "ANEModel.h"

ANEModel::ANEModel()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}


ANEModel::ANEModel(const ANEModel& other)
{
}


ANEModel::~ANEModel()
{
}

bool ANEModel::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* texturePath)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, texturePath);
	if (!result)
	{
		return false;
	}

	return true;
}

void ANEModel::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

void ANEModel::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}

ID3D11ShaderResourceView* ANEModel::GetTexture()
{
	return m_texture->GetTexture();
}

int ANEModel::GetIndexCount()
{
	return m_indexCount;
}

bool ANEModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	vertices[1].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);

	m_vertexBuffer = new ANEVertexBuffer<VertexType>();
	
	m_vertexBuffer->PushVertex(vertices[0]);
	m_vertexBuffer->PushVertex(vertices[1]);
	m_vertexBuffer->PushVertex(vertices[2]);
	m_vertexBuffer->PushVertex(vertices[3]);

	m_vertexBuffer->Create(device);

	m_indexBuffer = new ANEIndexBuffer();
	m_indexBuffer->PushTriangle(0,1,2);
	m_indexBuffer->PushTriangle(1,3,2);
	m_indexBuffer->Create(device);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	return true;
}

void ANEModel::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ANEModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	m_texture->Apply(deviceContext);

	m_vertexBuffer->Apply(deviceContext);
	m_indexBuffer->Apply(deviceContext);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ANEModel::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_texture = new ANETexture;

	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ANEModel::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		m_texture->Release();
		delete m_texture;
		m_texture = 0;
	}

	return;
}