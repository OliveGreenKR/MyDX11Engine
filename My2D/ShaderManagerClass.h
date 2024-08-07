#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "ShaderParameters.h"

using namespace DirectX;
using namespace std;

class ShaderClass 
{
public:
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext*, int indexCount, ShaderParameters parameters) = 0;

private:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR* vsfilename, WCHAR* psfilename) = 0;
	virtual void ShutdownShader() = 0;
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, ShaderParameters parameters);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
};