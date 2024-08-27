#include "ShaderClass.h"

struct LightShaderParameters
{
    ID3D11ShaderResourceView* baseTexture;

    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;

    XMFLOAT4 ambientColor;
    XMFLOAT4 diffuseColor;
    XMFLOAT3 lightDirection;
};

class LightShaderClass : public ShaderClass<LightShaderParameters>
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
#pragma pack(pop)
public:
    LightShaderClass();
    LightShaderClass(const LightShaderClass&) = delete;
    ~LightShaderClass();

    void Shutdown() override;
    bool Initialize(ID3D11Device* device, HWND hwnd) override;

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, LightShaderParameters& parameters) override;
    void ShutdownShader();

private:
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
};