#include "Applicationclass.h"
ApplicationClass::ApplicationClass()
{
	m_Direcrt3D = 0;	
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direcrt3D = new D3DClass;
	result = m_Direcrt3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}


void ApplicationClass::Shutdown()
{
	if(m_Direcrt3D) {
		m_Direcrt3D->Shutdown();
		delete m_Direcrt3D;
		m_Direcrt3D = 0;
	}
	return;
}


bool ApplicationClass::Frame()
{
	bool result;

	result = Render();	
	if(!result) {
		return false;
	}
	return true;
}


bool ApplicationClass::Render()
{
	//clear the buffers to begin the scene
	m_Direcrt3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	//present the rendered scene to the screen
	m_Direcrt3D->EndScene();

	return true;
}
