#pragma once
#include "ShaderClass.h"

struct WaterShaderParameters
{
    ID3D11ShaderResourceView* reflectionTexture;
    ID3D11ShaderResourceView* refractionTexture;
    ID3D11ShaderResourceView* normalTexture;

    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
    XMMATRIX reflection;

    float waterTranslation;
    float reflectRefractScale;
};

class WaterShaderClass : public ShaderClass<WaterShaderParameters>
{
private:
#pragma pack(push, 16)
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct ReflectionBufferType
    {
        XMMATRIX reflection;
    };

    struct WaterBufferType
    {
        float waterTranslation;
        float reflectRefractScale;
        XMFLOAT2 padding;
    };
#pragma pack(pop)
public:
    WaterShaderClass();
    WaterShaderClass(const WaterShaderClass&) = delete;
    ~WaterShaderClass();

    void Shutdown() override;
    bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfilename, WCHAR* psfilename) override;
    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, WaterShaderParameters& parameters) override;
    void ShutdownShader();

private:
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_reflectionBuffer;
    ID3D11Buffer* m_waterBuffer;
};