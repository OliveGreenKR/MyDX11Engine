#include "DDDAplicationClass.h"
#include "Define.h"
#include <string>

DDDApplicationClass::DDDApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = nullptr;
}


DDDApplicationClass::DDDApplicationClass(const DDDApplicationClass& other)
{
}


DDDApplicationClass::~DDDApplicationClass()
{
}


bool DDDApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128];
	char textureFilename[128];
	bool result;

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -10.0f);

	// Set the file name of the model.
	strcpy_s(modelFilename, MODEL_SPHERE_PATH);
	//strcpy_s(modelFilename, MODEL_PLANE_PATH);
	//strcpy_s(modelFilename, MODEL_CUBE_PATH);

	// Set the name of the texture file that we will be loading.
	strcpy_s(textureFilename, TEXTURE_STONE01_PATH);

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.

	m_Lights = vector<LightClass>(NUM_LIGHTS);

	float r, g, b;
	r = g = b = 1.f;
	m_Lights[0].SetDiffuseColor(r, g, b, 1.0f);
	m_Lights[0].SetAmbientColor(0.15f * r, 0.15f * g, 0.15f * b, 1.0f);
	m_Lights[0].SetWorldPosition(3.0f, 0.0f, -3.0f);
	m_Lights[0].SetSpecularColor(r, g, b, 1.0f);
	m_Lights[0].SetSpecularPower(32.0f);
	m_Lights[0].SetAttenuation(0, 0.f, 0);
	m_Lights[0].SetRange(120.f);

	r = 1.f;
	g = b = 0.f;
	m_Lights[1].SetDiffuseColor(r, g, b, 1.0f);
	m_Lights[1].SetAmbientColor(0.15f * r, 0.15f * g, 0.15f * b, 1.0f);
	m_Lights[1].SetWorldPosition(3.0f, 3.0f, -3.0f);
	m_Lights[1].SetSpecularColor(r, g, b, 1.0f);
	m_Lights[1].SetSpecularPower(32.0f);
	m_Lights[1].SetAttenuation(0.5f, 0, 0);
	m_Lights[1].SetRange(20.f);


	g = 1.f;
	r = b = 0.f;
	m_Lights[2].SetDiffuseColor(r, g, b, 1.0f);
	m_Lights[2].SetAmbientColor(0.15f * r, 0.15f * g, 0.15f * b, 1.0f);
	m_Lights[2].SetWorldPosition(-3.0f, -2.0f, -3.0f);
	m_Lights[2].SetSpecularColor(r, g, b, 1.0f);
	m_Lights[2].SetSpecularPower(32.0f);
	m_Lights[2].SetAttenuation(0.2f, 0.0f, 0);
	m_Lights[2].SetRange(20.f);


	b = 1.f;
	r = g = 0.f;
	m_Lights[3].SetDiffuseColor(r, g, b, 1.0f);
	m_Lights[3].SetAmbientColor(0.15f * r, 0.15f * g, 0.15f * b, 1.0f);
	m_Lights[3].SetWorldPosition(0.0f, 15.0f, -15.0f);
	m_Lights[3].SetSpecularColor(r, g, b, 1.0f);
	m_Lights[3].SetSpecularPower(32.0f);
	m_Lights[3].SetAttenuation(0, 0, 0);
	m_Lights[3].SetRange(5.f);


	return true;


}

void DDDApplicationClass::Shutdown()
{
	// Release the light object.
	if (!m_Lights.empty())
	{
		m_Lights.clear();
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}


bool DDDApplicationClass::Frame()
{
	static float rotation = 360.0f;
	static float speed = 0.5f;
	bool result;


	// Update the rotation variable each frame.


	rotation -= DEG_TO_RAD * speed;
	if (rotation < 0.0f)
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


bool DDDApplicationClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);  // Build the translation matrix.

	// Multiply them together to create the final world transformation matrix.
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	LightShaderClass::ShaderParameters shaderParameters;
	shaderParameters.texture = m_Model->GetTexture();
	shaderParameters.world = worldMatrix;
	shaderParameters.view = viewMatrix;
	shaderParameters.projection = projectionMatrix;
	shaderParameters.cameraPosition = m_Camera->GetPosition();

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		shaderParameters.ambientColor[i] = m_Lights[i].GetAmbientColor();
		shaderParameters.diffuseColor[i] = m_Lights[i].GetDiffuseColor();
		shaderParameters.lightPosition[i] = m_Lights[i].GetWorldPosition();
		shaderParameters.specularPower[i] = m_Lights[i].GetSpecularPower();
		shaderParameters.specularColor[i] = m_Lights[i].GetSpecularColor();
		shaderParameters.attenuation[i] = m_Lights[i].GetAttenuation();
		shaderParameters.range[i] = m_Lights[i].GetRange();
	}

	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), shaderParameters);
	if (!result)
	{
		return false;
	}

	//present the rendered scene to the screen
	m_Direct3D->EndScene();

	return true;
}
