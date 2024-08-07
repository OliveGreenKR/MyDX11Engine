#include "Applicationclass.h"
#include "Define.h"
#include <string>

ApplicationClass::ApplicationClass() 
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_mainShader = nullptr;
	m_pointLightShader = nullptr;
	m_Model = nullptr;
	m_FontShader = nullptr;
	m_Font = nullptr;
	m_Fps = nullptr;
	m_FpsString = nullptr;
	m_Light = nullptr;
	m_PointLight = nullptr;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char fpsString[32];
	char modelFilename[128], textureFilename1[128], textureFilename2[128], textureFilename3[128];
	char* textureFilenames[8];
	bool result;

#pragma region core
	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->Render();

	// Create and initialize the font shader object.
	m_FontShader = new FontShaderClass;
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the font object.
	m_Font = new FontClass;
	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		return false;
	}
#pragma endregion
#pragma region Fps
	// Create and initialize the fps object.
	m_Fps = new FpsClass();

	m_Fps->Initialize();

	// Set the initial fps and fps string.
	m_previousFps = -1;
	strcpy_s(fpsString, "Fps: 0");

	// Create and initialize the text object for the fps string.
	m_FpsString = new TextClass;

	result = m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, fpsString, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}
#pragma endregion
#pragma region Shader
	// Create and initialize the multitexture shader object.
	m_mainShader = new NormalMapShaderClass;
	m_pointLightShader = new PointLightShaderClass;

	result = m_mainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}
	result = m_pointLightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}
#pragma endregion
#pragma region Model
	// Set the file name of the model.
	strcpy_s(modelFilename, MODEL_CUBE_PATH);

	// Set the file name of the textures.
	strcpy_s(textureFilename1, TEXTURE_STONE01_PATH);
	strcpy_s(textureFilename2, NORMALMAP_STONE01_PATH);
	strcpy_s(textureFilename3, SPEC_STONE03_PATH);

	textureFilenames[0] = textureFilename1;
	textureFilenames[1] = textureFilename2;
	textureFilenames[2] = textureFilename3;

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, 2, textureFilenames);
	if (!result)
	{
		return false;
	}
#pragma endregion
#pragma region Lights
	m_Light = new LightClass;
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f,0.0f,1.0f,1.0f);
	m_Light->SetSpecularPower(16.0f);
	
	m_PointLight = new PointLightClass;
	m_PointLight->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_PointLight->SetPosition(0.0f, 0.0f, 1.0f);
	m_PointLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_PointLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight->SetSpecularPower(32.0f);
	m_PointLight->SetAttenuation(0.0f, 0.1f, 0.0f);
	m_PointLight->SetRange(10.0f);
#pragma endregion
	return true;

}

void ApplicationClass::Shutdown()
{
	if(m_Light) 
	{
		delete m_Light;
		m_Light = nullptr;
	}
	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the shader object.
	if (m_mainShader)
	{
		m_mainShader->Shutdown();
		delete m_mainShader;
		m_mainShader = 0;
	}

	// Release the text object for the fps string.
	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if(m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool ApplicationClass::Frame(InputClass* Input)
{
	bool result;
	static float rotation = 360.f;
#pragma region Fps
	result = UpdateFps();
	if (!result)
	{
		return false;
	}
#pragma endregion
#pragma region Input
	if(Input->IsEscapePressed())
	{
		return false;
	}
#pragma endregion

	// Update the rotation variable each frame.
	rotation -= DEG_TO_RAD * 0.25f;
	if (rotation <= 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	int i;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//m_Direct3D->EnableAlphaBlending();

#pragma region Fps
	// Render the fps text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
								  m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}
#pragma endregion

#pragma region Contents

	//rotate
	worldMatrix = XMMatrixRotationY(rotation);

	// Render the model using the multitexture shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	NormalMapShaderParameters parameters;

	parameters.baseTexture = m_Model->GetTexture(0);
	parameters.normalMap = m_Model->GetTexture(1);
	parameters.world = worldMatrix;
	parameters.view = viewMatrix;
	parameters.projection = projectionMatrix;
	parameters.diffuseColor = m_Light->GetDiffuseColor();
	parameters.lightDirection = m_Light->GetDirection();

	result = m_mainShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), parameters );
	if (!result)
	{
		return false;
	}

	PointLightShaderParameters plParameters;
	worldMatrix =  worldMatrix + XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	plParameters.world = worldMatrix;
	plParameters.view = viewMatrix;
	plParameters.projection = projectionMatrix;
	plParameters.cameraPosition = m_Camera->GetPosition();
	plParameters.lightCount = 1;

	for (int i = 0; i < plParameters.lightCount; i++) {
		plParameters.ambientColor[i] = m_PointLight->GetAmbientColor();
		plParameters.diffuseColor[i] = m_PointLight->GetDiffuseColor();
		plParameters.lightPosition[i] = m_PointLight->GetPosition();
		plParameters.specularPower[i] = m_PointLight->GetSpecularPower();
		plParameters.specularColor[i] = m_PointLight->GetSpecularColor();
		plParameters.attenuation[i] = m_PointLight->GetAttenuation();
		plParameters.range[i] = m_PointLight->GetRange();
	}

	result = m_pointLightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), plParameters);
	if (!result)
	{
		return false;
	}

#pragma endregion

	//m_Direct3D->DisableAlphaBlending();
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::UpdateFps()
{
	int fps;
	char tempString[16], finalString[16];
	float red, green, blue;
	bool result;


	// Update the fps each frame.
	m_Fps->Frame();

	// Get the current fps.
	fps = m_Fps->GetFps();

	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	sprintf_s(tempString, "%d", fps);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_FpsString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, -100, 10, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}
