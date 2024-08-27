#pragma once

#include "textureshaderclass.h"
#include "PointLightShaderClass.h"
#include "normalmapshaderclass.h"
#include "LightShaderClass.h"
#include "FogShaderClass.h"
#include "ReflexShaderClass.h"
#include "WaterShaderClass.h"
#include "RefractionShaderClass.h"

enum ShaderType
{
    TEXTURE,
	POINT_LIGHT,
    LIGHT,
	NORMAL_MAP,
    WATER,
    REFRACTION,
};

class ShaderManagerClass
{
public:
    ShaderManagerClass();
    ShaderManagerClass(const ShaderManagerClass&) = delete;
    ~ShaderManagerClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool RenderShader(ID3D11DeviceContext*, int, ShaderType type, void* parameters);


private:
    TextureShaderClass* m_TextureShader;
    LightShaderClass* m_LightShader;
    PointLightShaderClass* m_PointLightShader;
    NormalMapShaderClass* m_NormalMapShader;
    WaterShaderClass* m_WaterShader;
    RefractionShaderClass* m_RefractionShader;
};