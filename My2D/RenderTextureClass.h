#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class RenderTextureClass
{
public:
    RenderTextureClass();
    RenderTextureClass(const RenderTextureClass&);
    ~RenderTextureClass();

    bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format);
    void Shutdown();

    void SetRenderTarget(ID3D11DeviceContext*);
    void ClearRenderTarget(ID3D11DeviceContext*, float r, float g, float b, float a);

    ID3D11ShaderResourceView* GetShaderResourceView() { return m_shaderResourceView; }

    void GetProjectionMatrix(XMMATRIX& out) { out = m_projectionMatrix; }
    void GetOrthoMatrix(XMMATRIX& out) { out = m_orthoMatrix; }

    int GetTextureWidth() { return m_textureWidth; }
    int GetTextureHeight() { return m_textureHeight; }

private:
    int m_textureWidth, m_textureHeight;
    ID3D11Texture2D* m_renderTargetTexture;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_shaderResourceView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilView* m_depthStencilView;
    D3D11_VIEWPORT m_viewport;
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_orthoMatrix;
};

