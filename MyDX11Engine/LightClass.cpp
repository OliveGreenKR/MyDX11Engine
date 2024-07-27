#include "LightClass.h"

LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}


void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
    m_ambientColor = XMFLOAT4(r,g,b,a);
    return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
    return;
}


void LightClass::SetWorldPosition(float x, float y, float z)
{
    m_worldPosition = XMFLOAT3(x, y, z);
    return;
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
    m_specularColor = XMFLOAT4(r, g, b, a); 
    return;
}

void LightClass::SetSpecularPower(float power)
{
    m_specularPower = power;
    return;
}


XMFLOAT4 LightClass::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
    return m_diffuseColor;
}


XMFLOAT3 LightClass::GetWorldPosition()
{
    return m_worldPosition;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
    return m_specularColor;
}

float LightClass::GetSpecularPower()
{
    return m_specularPower;
}
