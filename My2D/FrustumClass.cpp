#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
}


FrustumClass::FrustumClass(const FrustumClass& other)
{
	for (int i = 0; i < 6; i++)
		m_planes[i] = other.m_planes[i];
	return;
}


FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix)
{
    // Combine the view and projection matrices
    XMMATRIX frustumMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

    // Left plane
    m_planes[0].x = frustumMatrix.r[0].m128_f32[3] + frustumMatrix.r[0].m128_f32[0];
    m_planes[0].y = frustumMatrix.r[1].m128_f32[3] + frustumMatrix.r[1].m128_f32[0];
    m_planes[0].z = frustumMatrix.r[2].m128_f32[3] + frustumMatrix.r[2].m128_f32[0];
    m_planes[0].w = frustumMatrix.r[3].m128_f32[3] + frustumMatrix.r[3].m128_f32[0];

    // Right plane
    m_planes[1].x = frustumMatrix.r[0].m128_f32[3] - frustumMatrix.r[0].m128_f32[0];
    m_planes[1].y = frustumMatrix.r[1].m128_f32[3] - frustumMatrix.r[1].m128_f32[0];
    m_planes[1].z = frustumMatrix.r[2].m128_f32[3] - frustumMatrix.r[2].m128_f32[0];
    m_planes[1].w = frustumMatrix.r[3].m128_f32[3] - frustumMatrix.r[3].m128_f32[0];

    // Top plane
    m_planes[2].x = frustumMatrix.r[0].m128_f32[3] - frustumMatrix.r[0].m128_f32[1];
    m_planes[2].y = frustumMatrix.r[1].m128_f32[3] - frustumMatrix.r[1].m128_f32[1];
    m_planes[2].z = frustumMatrix.r[2].m128_f32[3] - frustumMatrix.r[2].m128_f32[1];
    m_planes[2].w = frustumMatrix.r[3].m128_f32[3] - frustumMatrix.r[3].m128_f32[1];

    // Bottom plane
    m_planes[3].x = frustumMatrix.r[0].m128_f32[3] + frustumMatrix.r[0].m128_f32[1];
    m_planes[3].y = frustumMatrix.r[1].m128_f32[3] + frustumMatrix.r[1].m128_f32[1];
    m_planes[3].z = frustumMatrix.r[2].m128_f32[3] + frustumMatrix.r[2].m128_f32[1];
    m_planes[3].w = frustumMatrix.r[3].m128_f32[3] + frustumMatrix.r[3].m128_f32[1];

    // Near plane
    m_planes[4].x = frustumMatrix.r[0].m128_f32[2];
    m_planes[4].y = frustumMatrix.r[1].m128_f32[2];
    m_planes[4].z = frustumMatrix.r[2].m128_f32[2];
    m_planes[4].w = frustumMatrix.r[3].m128_f32[2];

    // Far plane
    m_planes[5].x = frustumMatrix.r[0].m128_f32[3] - frustumMatrix.r[0].m128_f32[2];
    m_planes[5].y = frustumMatrix.r[1].m128_f32[3] - frustumMatrix.r[1].m128_f32[2];
    m_planes[5].z = frustumMatrix.r[2].m128_f32[3] - frustumMatrix.r[2].m128_f32[2];
    m_planes[5].w = frustumMatrix.r[3].m128_f32[3] - frustumMatrix.r[3].m128_f32[2];

    // Normalize the planes
    for (int i = 0; i < 6; i++)
    {
        XMVECTOR plane = XMLoadFloat4(&m_planes[i]);
        plane = XMPlaneNormalize(plane);
        XMStoreFloat4(&m_planes[i], plane);
    }

}

bool FrustumClass::CheckPoint(float x , float y , float z)
{
    XMVECTOR point = XMVectorSet(x, y, z, 1.0f);

    return CheckPoint(point);
}

bool FrustumClass::CheckPoint(CXMVECTOR point)
{
    // Check if the point is inside all six planes of the view frustum.
    for (int i = 0; i < 6; i++)
    {
        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), point)) < 0.0f)
        {
            return false;
        }
    }
    return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
    // 8 points of the cube
    XMVECTOR point1 = XMVectorSet(xCenter - radius, yCenter - radius, zCenter - radius, 1.0f);
    XMVECTOR point2 = XMVectorSet(xCenter + radius, yCenter - radius, zCenter - radius, 1.0f);
    XMVECTOR point3 = XMVectorSet(xCenter - radius, yCenter + radius, zCenter - radius, 1.0f);
    XMVECTOR point4 = XMVectorSet(xCenter + radius, yCenter + radius, zCenter - radius, 1.0f);
    XMVECTOR point5 = XMVectorSet(xCenter - radius, yCenter - radius, zCenter + radius, 1.0f);
    XMVECTOR point6 = XMVectorSet(xCenter + radius, yCenter - radius, zCenter + radius, 1.0f);
    XMVECTOR point7 = XMVectorSet(xCenter - radius, yCenter + radius, zCenter + radius, 1.0f);
    XMVECTOR point8 = XMVectorSet(xCenter + radius, yCenter + radius, zCenter + radius, 1.0f);

    // Check if all 8 points are inside the view frustum
    if (CheckPoint(point1) && CheckPoint(point2) && CheckPoint(point3) &&
        CheckPoint(point4) && CheckPoint(point5) && CheckPoint(point6) &&
        CheckPoint(point7) && CheckPoint(point8))
    {
        return true;
    }

    return false;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float ZCenter, float radius)
{
    for (int i = 0; i < 6; i++) 
    {
        XMVECTOR plane = XMLoadFloat4(&m_planes[i]);
        if (XMVectorGetX(XMPlaneDotCoord(plane, XMVectorSet(xCenter, yCenter, ZCenter, 1.0f))) < -radius)
            return false;
    }
    return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{

    // 8 points of the rectangle
    XMVECTOR point1 = XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter - zSize, 1.0f);
    XMVECTOR point2 = XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter - zSize, 1.0f);
    XMVECTOR point3 = XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter - zSize, 1.0f);
    XMVECTOR point4 = XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter - zSize, 1.0f);
    XMVECTOR point5 = XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter + zSize, 1.0f);
    XMVECTOR point6 = XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter + zSize, 1.0f);
    XMVECTOR point7 = XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter + zSize, 1.0f);
    XMVECTOR point8 = XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter + zSize, 1.0f);

    // Check if all 8 points are inside the view frustum
    if (CheckPoint(point1) && CheckPoint(point2) && CheckPoint(point3) &&
        CheckPoint(point4) && CheckPoint(point5) && CheckPoint(point6) &&
        CheckPoint(point7) && CheckPoint(point8))
    {
        return true;
    }

    return false;
}

bool FrustumClass::CheckAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    // Calculate the center of the AABB
    XMFLOAT3 center(
        (minX + maxX) * 0.5f,
        (minY + maxY) * 0.5f,
        (minZ + maxZ) * 0.5f
    );

    // Calculate the extents (half-sizes) of the AABB
    XMFLOAT3 extents(
        (maxX - minX) * 0.5f,
        (maxY - minY) * 0.5f,
        (maxZ - minZ) * 0.5f
    );

    // Check each plane of the frustum
    for (int i = 0; i < 6; i++)
    {
        XMVECTOR plane = XMLoadFloat4(&m_planes[i]);
        XMVECTOR planeNormal = XMVectorSetW(plane, 0.0f); // Ignore the w component for the normal

        // Calculate the positive and negative extents
        float r = extents.x * fabsf(XMVectorGetX(planeNormal)) +
            extents.y * fabsf(XMVectorGetY(planeNormal)) +
            extents.z * fabsf(XMVectorGetZ(planeNormal));

        // Calculate the distance from the center to the plane
        float d = XMVectorGetX(XMPlaneDotCoord(plane, XMLoadFloat3(&center)));

        // If the distance is less than the negative extent, the AABB is outside the frustum
        if (d < -r)
        {
            return false;
        }
    }
    return true;
}
