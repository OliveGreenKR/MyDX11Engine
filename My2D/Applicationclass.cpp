#include "Applicationclass.h"
#include "Define.h"
#include <string>

ApplicationClass::ApplicationClass() 
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_ShaderManager = nullptr;

	m_GroundModel = nullptr;
	m_BathModel = nullptr;
	m_WallModel = nullptr;
	m_WaterModel = nullptr;

	m_FontShader = nullptr;
	m_Font = nullptr;
	m_Fps = nullptr;
	m_FpsString = nullptr;
	m_Light = nullptr;
	m_PointLights = nullptr;
	m_Timer = nullptr;
	m_Frustum = nullptr;

	m_RefractionTexture = nullptr;
	m_ReflectionTexture = nullptr;
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

	m_waterHeight = 2.75f;
	m_waterTranslation = 0.0f;

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
	m_Camera->SetPosition(0.0f, 0.f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_baseViewMatrix);

	m_Camera->SetPosition(0.0f, 6.0f, -18.0f);
	//m_Camera->SetEulerRotation(15.f, 0, 0);

	m_CameraTarget = new Transform;
	m_CameraTarget->SetPosition(0.0f, 3.f, 0.f);
	m_Camera->SetTarget(m_CameraTarget);

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
#pragma region RenderingCountText
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
	//strcpy_s(modelFilename, MODEL_CUBE2_PATH);
	//strcpy_s(textureFilename1, TEXTURE_STONE02_PATH);
	//strcpy_s(textureFilename2, NORMALMAP_STONE01_PATH);
	//strcpy_s(textureFilename3, SPEC_STONE03_PATH);

	textureFilenames[0] = textureFilename1;
	textureFilenames[1] = textureFilename2;
	textureFilenames[2] = textureFilename3;

	strcpy_s(modelFilename, "../My2D/data/ground.txt");
	strcpy_s(textureFilename1, "../My2D/data/ground01.tga");

	// Create and initialize the model object.
	m_GroundModel = new ModelClass;

	result = m_GroundModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, 1, textureFilenames);
	if (!result)
	{
		return false;
	}
	m_GroundModel->GetTransform()->SetPosition(0,1,0);

	strcpy_s(modelFilename, "../My2D/data/wall.txt");
	strcpy_s(textureFilename1, "../My2D/data/wall01.tga");

	m_WallModel = new ModelClass;
	
	result = m_WallModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, 1, textureFilenames);
	if (!result)
	{
		return false;
	}

	m_WallModel->GetTransform()->SetPosition(0, 6.0f, 8.0f);

	strcpy_s(modelFilename, "../My2D/data/bath.txt");
	strcpy_s(textureFilename1, "../My2D/data/marble01.tga");

	m_BathModel = new ModelClass;

	result = m_BathModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, 1, textureFilenames);
	if (!result)
	{
		return false;
	}

	m_BathModel->GetTransform()->SetPosition(0,2.0f, 0);	

	strcpy_s(modelFilename, "../My2D/data/water.txt");
	strcpy_s(textureFilename1, "../My2D/data/water01.tga");

	m_WaterModel = new ModelClass;


	result = m_WaterModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, 1, textureFilenames);
	if (!result)
	{
		return false;
	}

	m_WaterModel->GetTransform()->SetPosition(0, m_waterHeight, 0);

#pragma endregion
#pragma region Frustum
	m_Frustum = new FrustumClass;
#pragma endregion
#pragma region Lights
	m_Light = new LightClass;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.f, 1.f, 1.0f, 1.0f);
	m_Light->SetDirection(0,-2, -1);

	lightCount = max(4,MAX_LIGHTS);

	m_PointLights = new PointLightClass*[lightCount];

	for (int i = 0; i < lightCount; i++) {
		m_PointLights[i] = new PointLightClass;
		m_PointLights[i]->SetDiffuseColor(randF(), randF(), randF(), 1.0f);
		m_PointLights[i]->SetPosition(randF(10), randF(10), randF(10)+5);
		m_PointLights[i]->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_PointLights[i]->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_PointLights[i]->SetSpecularPower(32.0f);
		m_PointLights[i]->SetAttenuation(0.0f, 0.01f, 0.01f);
		m_PointLights[i]->SetRange(30.0f);
	}
	
#pragma endregion
#pragma region TextureRender
	m_RefractionTexture = new RenderTextureClass;
	result = m_RefractionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if(!result)
	{
		return false;
	}

	m_ReflectionTexture = new RenderTextureClass;
	result = m_ReflectionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		return false;
	}
#pragma endregion

	return true;

}

void ApplicationClass::Shutdown()
{
	// Release the render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}
	// Release the frustum class object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
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
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
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
	// Update the rotation variable each frame.
	rotation -= 0.25f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}
	float x = 10 * sqrt(2) * cos(XMConvertToRadians(rotation));
	float z = sqrt(200 - (x)*(x));

	z = rotation > 180 ? -z : z;
	m_Camera->SetPosition(x,6.0f, z);

	m_waterTranslation += 0.01f * m_Timer->GetTime();
	if(m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	result =  RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}
	
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, reflectionViewMatrix;
	bool renderModel, result;
	int i;
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	m_Direct3D->TurnZBufferOn();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

#pragma region Frustum
	m_Frustum->ConstructFrustum(viewMatrix, projectionMatrix);
#pragma endregion
#pragma region Contents

	XMMATRIX modelMatrix;
	modelMatrix = m_GroundModel->GetModelingMatrix();
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());
	result = RenderModelWithShader(LIGHT, m_GroundModel, modelMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	modelMatrix = m_WallModel->GetModelingMatrix();
	m_WallModel->Render(m_Direct3D->GetDeviceContext());
	result = RenderModelWithShader(LIGHT, m_WallModel, modelMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	modelMatrix = m_BathModel->GetModelingMatrix();
	m_BathModel->Render(m_Direct3D->GetDeviceContext());
	result = RenderModelWithShader(LIGHT, m_BathModel, modelMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	m_WaterModel->Render(m_Direct3D->GetDeviceContext());

	result = RenderModelWithShader(WATER, m_WaterModel, m_WaterModel->GetModelingMatrix(), viewMatrix, projectionMatrix);
	if (!result)
		return false;


#pragma endregion
#pragma region UI
	//for 2D rendering.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();
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
	// Present the rendered scene to the screen.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();
#pragma endregion

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

	// Update the sentence vertex buffer with the new string information.
	result = m_RenderCountString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::RenderRefractionToTexture()
{
	XMMATRIX modelMatrix, ViewMatrix, projectionMatrix;
	XMFLOAT4 clipPlane;
	bool result;

	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the render texture and clear it.
	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// use the camera to calculate the reflection view matrix
	XMFLOAT4 reflectionPlane(0.f, -1.0f, 0.0f, 1.5f);
	XMVECTOR planeVector = XMLoadFloat4(&reflectionPlane);

	m_Camera->Render();

	// Get the matrices.
	m_Direct3D->GetWorldMatrix(modelMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);


	modelMatrix = m_BathModel->GetModelingMatrix();
	// Render the model 
	m_BathModel->Render(m_Direct3D->GetDeviceContext());
	result = RenderModelWithShader(ShaderType::REFRACTION, m_BathModel, modelMatrix, ViewMatrix, projectionMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return result;
}

bool ApplicationClass::RenderReflectionToTexture()
{
	XMFLOAT4 reflectionPlane;
	XMMATRIX modelMatrix, reflectionViewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render texture and clear it.
	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	reflectionPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight);
	m_Camera->RenderReflection(reflectionPlane);
	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	m_Direct3D->GetWorldMatrix(modelMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	modelMatrix = m_WallModel->GetModelingMatrix();
	// Render the model 
	m_WallModel->Render(m_Direct3D->GetDeviceContext());
	result = RenderModelWithShader(ShaderType::LIGHT, m_WallModel, modelMatrix, reflectionViewMatrix, projectionMatrix);

	if (!result)
		return false;

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return result;
}

bool ApplicationClass::RenderModelWithShader(ShaderType type, ModelClass* model, XMMATRIX worldMatrix , XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;
	int indexCount =  model->GetIndexCount();

	switch (type) {
		case ShaderType::TEXTURE:
			TextureShaderParameters tParameters;
			tParameters.baseTexture = model->GetTexture(0);
			tParameters.world = worldMatrix;
			tParameters.view = viewMatrix;
			tParameters.projection = projectionMatrix;
			
			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, TEXTURE, &tParameters);
			break;
		case ShaderType::LIGHT:
			LightShaderParameters lParameters;
			lParameters.baseTexture = model->GetTexture(0);

			lParameters.world = worldMatrix;
			lParameters.view = viewMatrix;
			lParameters.projection = projectionMatrix;

			lParameters.ambientColor = m_Light->GetAmbientColor();
			lParameters.diffuseColor = m_Light->GetDiffuseColor();
			lParameters.lightDirection = m_Light->GetDirection();
			
			result =  m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, LIGHT, &lParameters);
			break;
		case ShaderType::NORMAL_MAP:
			NormalMapShaderParameters nomalParam;
			nomalParam.baseTexture = model->GetTexture(0);
			nomalParam.normalMap = model->GetTexture(1);
			nomalParam.diffuseColor = m_Light->GetDiffuseColor();
			nomalParam.lightDirection = m_Light->GetDirection();
			nomalParam.world = worldMatrix;
			nomalParam.view = viewMatrix;
			nomalParam.projection = projectionMatrix;
			
			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, NORMAL_MAP, &nomalParam);
			break;
		case ShaderType::POINT_LIGHT:
			PointLightShaderParameters plParameters;
			plParameters.baseTexture = model->GetTexture(0);
			plParameters.cameraPosition = m_Camera->GetPosition();
			plParameters.lightCount = lightCount;

			for (int i = 0; i < plParameters.lightCount; i++) {
				plParameters.ambientColor[i] = m_PointLights[i]->GetAmbientColor();
				plParameters.diffuseColor[i] = m_PointLights[i]->GetDiffuseColor();
				plParameters.lightPosition[i] = m_PointLights[i]->GetPosition();
				plParameters.specularPower[i] = m_PointLights[i]->GetSpecularPower();
				plParameters.specularColor[i] = m_PointLights[i]->GetSpecularColor();
				plParameters.attenuation[i] = m_PointLights[i]->GetAttenuation();
				plParameters.range[i] = m_PointLights[i]->GetRange();
			}
			plParameters.world = worldMatrix;
			plParameters.view = viewMatrix;
			plParameters.projection = projectionMatrix;
			
			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, POINT_LIGHT, &plParameters);
			break;
		case ShaderType::REFRACTION:
			RefractionShaderParameters refractionParameters;

			refractionParameters.baseTexture = model->GetTexture(0);
			
			refractionParameters.world = worldMatrix;
			refractionParameters.view = viewMatrix;
			refractionParameters.projection = projectionMatrix;
			
			refractionParameters.ambientColor = m_Light->GetAmbientColor();
			refractionParameters.diffuseColor = m_Light->GetDiffuseColor();
			refractionParameters.lightDirection = m_Light->GetDirection();

			refractionParameters.clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, REFRACTION, &refractionParameters);
			break;

		case ShaderType::WATER:
			WaterShaderParameters wParameters;
			wParameters.normalTexture = model->GetTexture(0);
			wParameters.reflectionTexture = m_ReflectionTexture->GetShaderResourceView();
			wParameters.refractionTexture = m_RefractionTexture->GetShaderResourceView();
			wParameters.waterTranslation = m_waterTranslation;
			wParameters.reflectRefractScale = 0.03f;
			wParameters.world = worldMatrix;
			wParameters.view = viewMatrix;
			wParameters.projection = projectionMatrix;
			m_Camera->GetReflectionViewMatrix(wParameters.reflection);

			result = m_ShaderManager->RenderShader(m_Direct3D->GetDeviceContext(), indexCount, WATER, &wParameters);
			break;

		default:
			result = false;
			break;
	}

	return result;
}