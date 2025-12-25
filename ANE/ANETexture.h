#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "ANEBuffer.h"
#include <fstream>
using namespace DirectX;
using namespace std;

class ANETexture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};


public:
	/// <summary>
	/// Creates a new texture
	/// </summary>
	ANETexture();

	/// <summary>
	/// Destroys the texture
	/// </summary>
	~ANETexture();

	/// <summary>
	/// Initialize the texture
	/// </summary>
	/// <param name="device">The D3D device</param>
	/// <param name="deviceContext">The D3D device context</param>
	/// <param name="filePath">The filepath to the texture</param>
	/// <returns>True if the texture was created</returns>
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filePath);

	/// <summary>
	/// Releases the texture
	/// </summary>
	void Release();

	/// <summary>
	/// Applies the texture
	/// </summary>
	/// <param name="deviceContext">The device context</param>
	void Apply(ID3D11DeviceContext* deviceContext);

    /// <summary>
    /// Gets the texture's data
    /// </summary>
    /// <returns>The texture</returns>
    ID3D11ShaderResourceView* GetTexture();
	
    /// <summary>
    /// Gets the texture width
    /// </summary>
    /// <returns>Its width</returns>
    int GetWidth();

    /// <summary>
    /// Gets the texture's height
    /// </summary>
    /// <returnsIts height</returns>
    int GetHeight();

private:
	/// <summary>
	/// Loads a .tga
	/// </summary>
	/// <param name="filePath">The filepath</param>
	/// <returns>True if the texture was loaded</returns>
	bool LoadTarga32Bit(char* filePath);

private:
	ID3D11SamplerState* m_sampleState;

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	int m_width, m_height;
};

