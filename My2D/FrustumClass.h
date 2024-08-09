#pragma once

#include <DirectXMath.h>
using namespace DirectX;


class FrustumClass
{
public:
    FrustumClass();
    FrustumClass(const FrustumClass&);
    ~FrustumClass();

    void ConstructFrustum(CXMMATRIX, CXMMATRIX);

    bool CheckPoint(float, float, float);
    bool CheckPoint(CXMVECTOR point);
    bool CheckCube(float, float, float, float);
    bool CheckSphere(float, float, float, float);
    bool CheckRectangle(float, float, float, float, float, float);
    bool CheckAABB(float, float, float, float, float, float);

private:
    XMFLOAT4 m_planes[6];
};
