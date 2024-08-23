#pragma once
#include "ShaderClass.h"

struct RefractionShaderParameters
{
    ID3D11ShaderResourceView* texture;

    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;

    XMFLOAT4 ambientColor;
    XMFLOAT4 diffuseColor;
    XMFLOAT3 lightDirection;

    XMFLOAT4 clipPlane;
};

class RefractionShaderClass : public ShaderClass<RefractionShaderParameters>
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
        XMFLOAT4 ambientColor;
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float padding; 
    };

    struct ClipPlaneBufferType
    {
        XMFLOAT4 clipPlane;
    };

#pragma pack(pop)
public:
    RefractionShaderClass();
    RefractionShaderClass(const RefractionShaderClass&) = delete;
    ~RefractionShaderClass();

    void Shutdown() override;
    bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, RefractionShaderParameters& parameters) override;
    void ShutdownShader();

private:
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_clipPlaneBuffer;
};