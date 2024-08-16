#pragma once
#include "ShaderClass.h"

struct ReflexShaderParameters
{
    ID3D11ShaderResourceView* baseTexture;
    ID3D11ShaderResourceView* reflectionTexture;

    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
    XMMATRIX reflection;
};

class ReflexShaderClass : public ShaderClass<ReflexShaderParameters>
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
#pragma pack(pop)
public:
    ReflexShaderClass();
    ReflexShaderClass(const ReflexShaderClass&) = delete;
    ~ReflexShaderClass();

    void Shutdown() override;
    bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfilename, WCHAR* psfilename) override;
    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, ReflexShaderParameters& parameters) override;
    void ShutdownShader();

private:
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_reflectionBuffer;
};
