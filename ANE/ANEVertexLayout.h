#pragma once

#include <directxmath.h>
#include <vector>
#include <d3d11.h>

using namespace DirectX;

struct VertexLayout_Position {
	// Constructor for ease of use
	VertexLayout_Position() = default;
	VertexLayout_Position(XMFLOAT4 const& pos) noexcept : position(pos) {}

	// The actual data inside the struct
	XMFLOAT4 position;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionColor {
	// Constructor for ease of use
	VertexLayout_PositionColor() = default;
	VertexLayout_PositionColor(XMFLOAT4 const& pos, XMFLOAT4 const& col) noexcept : position(pos), color(col) {}

	// The actual data inside the struct
	XMFLOAT4 position;
	XMFLOAT4 color;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionColorUV {
	// Constructor for ease of use
	VertexLayout_PositionColorUV() = default;
	VertexLayout_PositionColorUV(XMFLOAT4 const& pos, XMFLOAT4 const& col, XMFLOAT2 const& uv) noexcept : position(pos), color(col), uv(uv) {}

	// The actual data inside the struct
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMFLOAT2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionUV {
	// Constructor for ease of use
	VertexLayout_PositionUV() = default;
	VertexLayout_PositionUV(XMFLOAT4 const& pos, XMFLOAT2 const& uv) noexcept : position(pos), uv(uv) {}

	// The actual data inside the struct
	XMFLOAT4 position;
	XMFLOAT2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionNormalUV {
	// Constructor for ease of use
	VertexLayout_PositionNormalUV() = default;
	VertexLayout_PositionNormalUV(XMFLOAT4 const& pos, XMFLOAT4 const& normal, XMFLOAT2 const& uv) noexcept : position(pos), normal(normal), uv(uv) {}

	// The actual data inside the struct
	XMFLOAT4 position;
	XMFLOAT4 normal;
	XMFLOAT2 uv;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
};

struct VertexLayout_PositionNormalUVInstanced {
	// Constructor for ease of use
	VertexLayout_PositionNormalUVInstanced() = default;
	VertexLayout_PositionNormalUVInstanced(XMFLOAT4 const& pos, XMFLOAT4 const& normal, XMFLOAT2 const& uv, XMFLOAT3 const& instancePos) noexcept : position(pos), normal(normal), uv(uv), instancePos(instancePos) {}

	// The actual data inside the struct
	XMFLOAT4 position;
	XMFLOAT4 normal;
	XMFLOAT2 uv;
	XMFLOAT3 instancePos;

	// Input Layout Descriptor
	static inline const std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,    1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};
};