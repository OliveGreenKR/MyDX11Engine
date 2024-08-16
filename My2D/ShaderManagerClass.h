#pragma once

#include "textureshaderclass.h"
#include "PointLightShaderClass.h"
#include "normalmapshaderclass.h"
#include "FogShaderClass.h"
#include "ReflexShaderClass.h"

enum ShaderType
{
    TEXTURE,
	POINT_LIGHT,
	NORMAL_MAP,
    FOG,
    REFLEX,
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
    PointLightShaderClass* m_PointLightShader;
    NormalMapShaderClass* m_NormalMapShader;
    FogShaderClass* m_FogShader;
    ReflexShaderClass* m_ReflexShader;
};