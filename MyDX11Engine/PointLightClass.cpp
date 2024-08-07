#include "PointLightClass.h"
#include "PointLightClass.h"

PointLightClass::PointLightClass()
{
    return;
}

PointLightClass::PointLightClass(const PointLightClass& other)
{
}


PointLightClass::~PointLightClass()
{
}


void PointLightClass::SetAmbientColor(float r, float g, float b, float a)
{
    m_ambientColor = XMFLOAT4(r,g,b,a);
    return;
}

void PointLightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
    return;
}


void PointLightClass::SetWorldPosition(float x, float y, float z)
{
    m_worldPosition = XMFLOAT3(x, y, z);
    return;
}

void PointLightClass::SetSpecularColor(float r, float g, float b, float a)
{
    m_specularColor = XMFLOAT4(r, g, b, a); 
    return;
}

void PointLightClass::SetSpecularPower(float power)
{
    m_specularPower = power;
    return;
}

void PointLightClass::SetAttenuation(float x, float y, float z)
{
    XMFLOAT3 attenuation = XMFLOAT3(x, y, z);
    XMVECTOR vector = XMLoadFloat3(&attenuation);
    vector = XMVector3Normalize(vector);
    XMStoreFloat3(&m_attenuation, vector);
}

void PointLightClass::SetRange(float range)
{
    m_range = range;
    return;
}


XMFLOAT4 PointLightClass::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 PointLightClass::GetDiffuseColor()
{
    return m_diffuseColor;
}


XMFLOAT3 PointLightClass::GetWorldPosition()
{
    return m_worldPosition;
}

XMFLOAT4 PointLightClass::GetSpecularColor()
{
    return m_specularColor;
}

float PointLightClass::GetSpecularPower()
{
    return m_specularPower;
}

XMFLOAT3 PointLightClass::GetAttenuation()
{
    return m_attenuation;
}

float PointLightClass::GetRange()
{
    return m_range;
}
