#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

#include "Textureclass.h"

class ModelClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };

    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, int, char**);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();
    int GetTextureCount() { return m_textureCount;}
    ID3D11ShaderResourceView* GetTexture(int);
    ID3D11ShaderResourceView** GetTextures();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char**);
    void ReleaseTextures();

    bool LoadModel(char*);
    void ReleaseModel();

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount, m_textureCount;
    TextureClass* m_Textures;
    ModelType* m_model;
};