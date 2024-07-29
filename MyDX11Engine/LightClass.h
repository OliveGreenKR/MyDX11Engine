#pragma once
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <directxmath.h>

using namespace DirectX;

class LightClass
{
public:
    LightClass();
    LightClass(const LightClass&);
    ~LightClass();

    void SetAmbientColor(float, float, float, float);
    void SetDiffuseColor(float, float, float, float);
    void SetWorldPosition(float, float, float);
    void SetSpecularColor(float, float, float, float);
    void SetSpecularPower(float);
	void SetAttenuation(float, float, float);
    void SetRange(float range);

    XMFLOAT4 GetAmbientColor();
    XMFLOAT4 GetDiffuseColor();
    XMFLOAT3 GetWorldPosition();
    XMFLOAT4 GetSpecularColor();
    float GetSpecularPower();
    XMFLOAT3 GetAttenuation();
    float GetRange();

private:
    XMFLOAT4 m_ambientColor;
    XMFLOAT4 m_diffuseColor;
    XMFLOAT3 m_worldPosition;
    XMFLOAT4 m_specularColor;
    float m_specularPower;
    XMFLOAT3 m_attenuation;
    float m_range;
};

#endif
