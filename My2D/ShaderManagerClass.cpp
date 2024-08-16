#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TextureShader = nullptr;
	m_PointLightShader = nullptr;
	m_NormalMapShader = nullptr;
	m_FogShader = nullptr;
	m_ReflexShader = nullptr;
}

ShaderManagerClass::~ShaderManagerClass()
{
	Shutdown();
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
    // Create texture shaders and initialize them
    m_TextureShader = new TextureShaderClass;
    result = m_TextureShader->Initialize(device, hwnd);
    if (!result)
    {
        return false;
    }

	m_PointLightShader =  new PointLightShaderClass;
	result = m_PointLightShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	m_NormalMapShader = new NormalMapShaderClass;
	result = m_NormalMapShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	m_FogShader = new FogShaderClass;
	result = m_FogShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	m_ReflexShader = new ReflexShaderClass;
	result = m_ReflexShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

    return true;
}

void ShaderManagerClass::Shutdown()
{
	// Release the reflex shader object.
	if (m_ReflexShader)
	{
		m_ReflexShader->Shutdown();
		delete m_ReflexShader;
		m_ReflexShader = nullptr;
	}
	if(m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = nullptr;
	}

	if (m_NormalMapShader)
	{
		m_NormalMapShader->Shutdown();
		delete m_NormalMapShader;
		m_NormalMapShader = nullptr;
	}

	// Release the light shader object.
	if (m_PointLightShader)
	{
		m_PointLightShader->Shutdown();
		delete m_PointLightShader;
		m_PointLightShader = nullptr;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	return;
}

bool ShaderManagerClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, ShaderType type, void* parameters)
{
	switch (type)
	{
		case TEXTURE:
		{
			return m_TextureShader->Render(deviceContext, indexCount, *(TextureShaderParameters*)parameters);
		}
		case POINT_LIGHT:
		{
			return m_PointLightShader->Render(deviceContext, indexCount, *(PointLightShaderParameters*)parameters);
		}
		case NORMAL_MAP:
		{
			return m_NormalMapShader->Render(deviceContext, indexCount, *(NormalMapShaderParameters*)parameters);
		}
		case FOG:
		{
			return m_FogShader->Render(deviceContext, indexCount, *(FogShaderParameters*)parameters);
		}
		case REFLEX:
		{
			return m_ReflexShader->Render(deviceContext, indexCount, *(ReflexShaderParameters*)parameters);
		}
		default:
			return false;
	}
}

