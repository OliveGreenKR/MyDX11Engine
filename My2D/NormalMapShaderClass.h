#pragma once
#include "ShaderClass.h"

struct NormalMapShaderParameters
{
	ID3D11ShaderResourceView* baseTexture;
	ID3D11ShaderResourceView* normalMap;

	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;

};

class NormalMapShaderClass : public ShaderClass<NormalMapShaderParameters>

{
private:
#pragma pack(push, 16)
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
#pragma pack(pop)
public:
	NormalMapShaderClass();
	NormalMapShaderClass(const NormalMapShaderClass&) = delete;
	~NormalMapShaderClass();

	void Shutdown() override;
	bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfilename, WCHAR* psfilename) override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, NormalMapShaderParameters& parameters) override;
	void ShutdownShader();

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};
