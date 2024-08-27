#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TextureShader = nullptr;
	m_PointLightShader = nullptr;
	m_NormalMapShader = nullptr;
	m_LightShader = nullptr;
	m_WaterShader = nullptr;
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

	m_LightShader = new LightShaderClass;
	result = m_LightShader->Initialize(device, hwnd);
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

	m_WaterShader = new WaterShaderClass;
	result = m_WaterShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	m_RefractionShader = new RefractionShaderClass;
	result = m_RefractionShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

    return true;
}

void ShaderManagerClass::Shutdown()
{
	if (m_RefractionShader) 
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = nullptr;
	
	}
	if(m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = nullptr;
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

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
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
		case LIGHT:
		{
			return m_LightShader->Render(deviceContext, indexCount, *(LightShaderParameters*)parameters);
		}
		case POINT_LIGHT:
		{
			return m_PointLightShader->Render(deviceContext, indexCount, *(PointLightShaderParameters*)parameters);
		}
		case NORMAL_MAP:
		{
			return m_NormalMapShader->Render(deviceContext, indexCount, *(NormalMapShaderParameters*)parameters);
		}
		case WATER:
		{
			return m_WaterShader->Render(deviceContext, indexCount, *(WaterShaderParameters*)parameters);
		}
		case REFRACTION:
		{
			return m_RefractionShader->Render(deviceContext, indexCount, *(RefractionShaderParameters*)parameters);
		}
		default:
			return false;
	}
}

