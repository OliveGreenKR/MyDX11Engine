#include "Applicationclass.h"
#include "Define.h"
#include <string>

ApplicationClass::ApplicationClass() 
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_ShaderManager = nullptr;
	m_Model = nullptr;
	m_FontShader = nullptr;
	m_Font = nullptr;
	m_Fps = nullptr;
	m_FpsString = nullptr;
	m_Light = nullptr;
	m_PointLight = nullptr;
	m_Timer = nullptr;
	m_Frustum = nullptr;
	m_Position = nullptr;
	m_ModelList = nullptr;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char fpsString[32], renderCountString[32];
	char modelFilename[128], textureFilename1[128], textureFilename2[128], textureFilename3[128];
	char* textureFilenames[8];
	bool result;

#pragma region D3D, Camera, Font, FontShader, Timer
	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -15.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_baseViewMatrix);

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

	m_Timer = new TimerClass;
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer", L"Error", MB_OK);
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
#pragma region RenderingCount
	m_RenderCountString = new TextClass;
	strcpy_s(renderCountString, "Render count: 0");

	result =  m_RenderCountString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font,renderCountString, 10, 30, 0.0f, 1.0f, 0.0f);
#pragma endregion
#pragma region ShaderManager
	// Create and initialize the multitexture shader object.
	m_ShaderManager = new ShaderManagerClass;
	result =  m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}
#pragma endregion
#pragma region Model
	// Set the file name of the model.
	strcpy_s(modelFilename, MODEL_SPHERE_PATH);

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
#pragma region ModelList
	// Create and initialize the model list object.
	m_ModelList = new ModelListClass;
	m_ModelList->Initialize(25);
#pragma endregion
#pragma region Movement
	m_Position = new PositionClass;
#pragma endregion
#pragma region Frustum
	m_Frustum = new FrustumClass;
#pragma endregion
#pragma region Lights
	m_Light = new LightClass;
	m_Light->SetDiffuseColor(0.3f, 0.5f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f,0.0f,1.0f,1.0f);
	m_Light->SetSpecularPower(16.0f);
	
	m_PointLight = new PointLightClass;
	m_PointLight->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_PointLight->SetPosition(0.0f, 1.0f, -5.0f);
	m_PointLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_PointLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight->SetSpecularPower(32.0f);
	m_PointLight->SetAttenuation(0.0f, 0.01f, 0.001f);
	m_PointLight->SetRange(10.0f);
#pragma endregion
	return true;

}

void ApplicationClass::Shutdown()
{
	// Release the frustum class object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the model list object.
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the text objects for the render count string.
	if (m_RenderCountString)
	{
		m_RenderCountString->Shutdown();
		delete m_RenderCountString;
		m_RenderCountString = 0;
	}

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
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
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
	m_Timer->Frame();
#pragma region Texts
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
#pragma region Movement
	m_Position->SetFrameTime(m_Timer->GetTime());
	m_Position->TurnLeft(Input->IsLeftArrowPressed());
	m_Position->TurnRight(Input->IsRightArrowPressed());
	rotation = m_Position->GetRotationY();
#pragma endregion

	m_Camera->SetRotation(0.0f, rotation, 0.0f);	
	m_Camera->Render();

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	float positionX, positionY, positionZ, radius;
	bool renderModel, result;
	int modelCount, renderCount, i;
	NormalMapShaderParameters nomalParam;
	PointLightShaderParameters plParameters;
	TextureShaderParameters tParameters;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//m_Direct3D->EnableAlphaBlending();


#pragma region Frustum
	m_Frustum->ConstructFrustum(viewMatrix, projectionMatrix);
#pragma endregion
#pragma region Contents

	nomalParam.baseTexture = m_Model->GetTexture(0);
	nomalParam.normalMap = m_Model->GetTexture(1);
	nomalParam.diffuseColor = m_Light->GetDiffuseColor();
	nomalParam.lightDirection = m_Light->GetDirection();

	plParameters.baseTexture = m_Model->GetTexture(0);
	//plParameters.world = worldMatrix
	//plParameters.view = viewMatrix;
	//plParameters.projection = projectionMatrix;
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

	tParameters.baseTexture = m_Model->GetTexture(0);


	modelCount = m_ModelList->GetModelCount();
	renderCount = 0;
	for (int i = 0; i < modelCount; i++) {
		m_ModelList->GetData(i, positionX, positionY, positionZ);
		radius = 1.0f;

		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);
		//render models in Frustum
		if (renderModel) {
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);
			m_Model->Render(m_Direct3D->GetDeviceContext());
			nomalParam.world = worldMatrix;
			nomalParam.view = viewMatrix;
			nomalParam.projection = projectionMatrix;
			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), ShaderType::NORMAL_MAP, &nomalParam);
			if (!result)
			{
				return false;
			}
			renderCount++;
		}
	}

	result = UpdateRenderCount(renderCount);
	if(!result)
	{
		return false;
	}

#pragma endregion
	//for 2D rendering.
	m_Direct3D->EnableAlphaBlending();
	m_Direct3D->TurnZBufferOff();

	//reset world matrix
	m_Direct3D->GetWorldMatrix(worldMatrix);
#pragma region Fps
	// Render the fps text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
								  m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}
#pragma endregion
#pragma region RenderCount
	m_RenderCountString->Render(m_Direct3D->GetDeviceContext());
	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_RenderCountString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
								  m_Font->GetTexture(), m_RenderCountString->GetPixelColor());
#pragma endregion
	// Present the rendered scene to the screen.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();
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

bool ApplicationClass::UpdateRenderCount(int renderCount)
{
	char tempString[16], finalString[32];
	bool result;


	// Convert the render count integer to string format.
	sprintf_s(tempString, "%d", renderCount);

	// Setup the render count string.
	strcpy_s(finalString, "Render Count: ");
	strcat_s(finalString, tempString);
	strcat_s(finalString, " Pitch :");

	sprintf_s(tempString, "%.3f", m_Position->GetRotationY());
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_RenderCountString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}