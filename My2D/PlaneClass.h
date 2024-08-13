#pragma once
#include "d3dclass.h"

class DisplayPlaneClass
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    DisplayPlaneClass();
    DisplayPlaneClass(const DisplayPlaneClass&);
    ~DisplayPlaneClass();

    bool Initialize(ID3D11Device*, float, float);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount() { return m_indexCount; }

private:
    bool InitializeBuffers(ID3D11Device*, float, float);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount;
};

