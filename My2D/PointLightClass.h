#pragma once

#include <directxmath.h>

using namespace DirectX;

class PointLightClass
{
public:
    PointLightClass();
    PointLightClass(const PointLightClass&);
    ~PointLightClass();
public:
    XMFLOAT3 GetPosition() const { return m_position; }
    XMFLOAT4 GetAmbientColor() const { return m_ambientColor; }
    XMFLOAT4 GetDiffuseColor() const { return m_diffuseColor; }
    XMFLOAT4 GetSpecularColor() const { return m_specularColor; }
    XMFLOAT3 GetAttenuation() const { return m_attenuation; }
    float GetRange() const { return m_range; }
    float GetSpecularPower() const { return m_specularPower; }


	void SetPosition(float x, float y, float z) { m_position = XMFLOAT3(x, y, z); }
	void SetAmbientColor(float r, float g, float b, float a) { m_ambientColor = XMFLOAT4(r, g, b, a); }
	void SetDiffuseColor(float r, float g, float b, float a) { m_diffuseColor = XMFLOAT4(r, g, b, a); }
	void SetSpecularColor(float r, float g, float b, float a) { m_specularColor = XMFLOAT4(r, g, b, a); }
	void SetAttenuation(float x, float y, float z) { m_attenuation = XMFLOAT3(x, y, z); }
	void SetRange(float range) { m_range = range; }
	void SetSpecularPower(float power) { m_specularPower = power; }

private:
    XMFLOAT3 m_position;
    XMFLOAT4 m_ambientColor;
    XMFLOAT4 m_diffuseColor;
    XMFLOAT4 m_specularColor;
    XMFLOAT3 m_attenuation;
    float m_range;
    float m_specularPower;
};
