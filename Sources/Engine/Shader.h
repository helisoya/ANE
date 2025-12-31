#pragma once

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

/// <summary>
/// Represents a shader
/// </summary>
/// <typeparam name="TInputLayout">The inputLayout type</typeparam>
template<typename TInputLayout>
class Shader {
	std::wstring shaderName;
	std::vector<uint8_t> psBytecode;
	ComPtr<ID3D11InputLayout> inputLayout;

	ComPtr<ID3D11VertexShader>	vertexShader;
	ComPtr<ID3D11PixelShader>	pixelShader;
public:
	std::vector<uint8_t> vsBytecode;
	Shader(std::wstring name) : shaderName(name) {};

	/// <summary>
	/// Creates the shader
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Create(DeviceResources* deviceRes) {
		auto d3dDevice = deviceRes->GetD3DDevice();
		vsBytecode = DX::ReadData((std::wstring(L"Resources/Shaders/Compiled/") + shaderName + L"_vs.cso").c_str());
		psBytecode = DX::ReadData((std::wstring(L"Resources/Shaders/Compiled/") + shaderName + L"_ps.cso").c_str());

		d3dDevice->CreateVertexShader(vsBytecode.data(), vsBytecode.size(), nullptr, vertexShader.ReleaseAndGetAddressOf());
		d3dDevice->CreatePixelShader(psBytecode.data(), psBytecode.size(), nullptr, pixelShader.ReleaseAndGetAddressOf());

		deviceRes->GetD3DDevice()->CreateInputLayout(
			typename TInputLayout::InputElementDescs.data(),
			typename TInputLayout::InputElementDescs.size(),
			vsBytecode.data(),
			vsBytecode.size(),
			inputLayout.ReleaseAndGetAddressOf());
	}

	/// <summary>
	/// Applies the shader
	/// </summary>
	/// <param name="deviceRes">The game's device resources</param>
	void Apply(DeviceResources* deviceRes) {
		auto d3dContext = deviceRes->GetD3DDeviceContext();
		d3dContext->VSSetShader(vertexShader.Get(), nullptr, 0);
		d3dContext->PSSetShader(pixelShader.Get(), nullptr, 0);
		deviceRes->GetD3DDeviceContext()->IASetInputLayout(inputLayout.Get());
	}
};