#pragma once
#include "ShaderClass.h"
#define MAX_LIGHTS 4    

struct PointLightShaderParameters 
{
    ID3D11ShaderResourceView* baseTexture;

    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;

    XMFLOAT3 cameraPosition;
    
    int lightCount;
    XMFLOAT4 ambientColor[MAX_LIGHTS];
    XMFLOAT4 diffuseColor[MAX_LIGHTS];
    XMFLOAT3 lightPosition[MAX_LIGHTS];
    float specularPower[MAX_LIGHTS];
    XMFLOAT4 specularColor[MAX_LIGHTS];
    XMFLOAT3 attenuation[MAX_LIGHTS];
    float range[MAX_LIGHTS];
};

class PointLightShaderClass : public ShaderClass<PointLightShaderParameters>
{
#pragma pack(push, 16)
private:

    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct PointLight
    {
        XMFLOAT4 ambientColor;
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightPosition;
        float specularPower;
        XMFLOAT4 specularColor;
        XMFLOAT3 attenuation;
        float range;
    };

    struct PointLightBufferType
    {
        PointLight light[MAX_LIGHTS];
    };

    struct ConstantBufferType
    {
        XMFLOAT3 cameraPosition;
        int lightCount;
    };

#pragma pack(pop)
public:
    PointLightShaderClass();
    PointLightShaderClass(const PointLightShaderClass&) = delete;
    ~PointLightShaderClass();

    void Shutdown() override;
    bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfilename, WCHAR* psfilename) override;
    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, PointLightShaderParameters& parameters) override;
    void ShutdownShader();
private:
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_constantBuffer;
};

