
#pragma once

#include "ShaderClass.h"

struct TextureShaderParameters
{
	ID3D11ShaderResourceView* baseTexture;

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class TextureShaderClass : public ShaderClass<TextureShaderParameters>
{
private:
#pragma pack(push, 16)
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
#pragma pack(pop)

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&) = delete;
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;

private:
	void ShutdownShader();
	bool InitializeShader(ID3D11Device*, HWND, WCHAR* vsFilename, WCHAR* psFilename) override;
	bool SetShaderParameters(ID3D11DeviceContext*, TextureShaderParameters& parameters) override;

private:
	ID3D11Buffer* m_matrixBuffer;

};
