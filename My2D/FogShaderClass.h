#pragma once
#include "ShaderClass.h"

struct FogShaderParameters 
{
	ID3D11ShaderResourceView* baseTexture;

	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMFLOAT4 fogColor;
	float fogStart;
	float fogEnd;
	float fogDensity;
	XMFLOAT4 cameraPosition;
	int fogType;
};

class FogShaderClass : public ShaderClass<FogShaderParameters>
{
private:
#pragma pack(push, 16)
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct FogBufferType
	{
		XMFLOAT4 fogColor;
		float fogStart;
		float fogEnd;
		float fogDensity;
		XMFLOAT4 cameraPosition;
		int fogType;
	};

#pragma pack(pop)
public:
	// Inherited via ShaderClass
	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool SetShaderParameters(ID3D11DeviceContext*, FogShaderParameters& parameters) override;
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR* vsFilename, WCHAR* psFilename) override;
	void ShutdownShader();

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_fogBuffer;


};

