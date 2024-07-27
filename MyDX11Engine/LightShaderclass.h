#pragma once
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_
#define NUM_LIGHTS 4

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;
using namespace std;


class LightShaderClass
{

#pragma pack(push, 16)
private:

    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct Light
    {
        XMFLOAT4 ambientColor;
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightPosition;
        float specularPower;
        XMFLOAT4 specularColor;
    };

    struct LightBufferType
    {
		Light light[NUM_LIGHTS];
    };

    struct CameraBufferType
    {
        XMFLOAT3 cameraPosition;
        float padding;
    };

#pragma pack(pop)
public:
    struct ShaderParameters 
    {
        ID3D11ShaderResourceView* texture;

        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;

        XMFLOAT3 cameraPosition;

        XMFLOAT4 ambientColor[NUM_LIGHTS];
        XMFLOAT4 diffuseColor[NUM_LIGHTS];
        XMFLOAT3 lightPosition[NUM_LIGHTS];
        float specularPower[NUM_LIGHTS];
        XMFLOAT4 specularColor[NUM_LIGHTS];
    };

public:
    LightShaderClass();
    LightShaderClass(const LightShaderClass&);
    ~LightShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, OUT ShaderParameters& );

private:
    bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, OUT ShaderParameters&);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_cameraBuffer;   

};
#endif
