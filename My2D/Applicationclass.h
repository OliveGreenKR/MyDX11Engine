#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#define DEFAULT_WINDOW_WIDTH (800)
#define DEFAULT_WINDOW_HEIGHT (600)

#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"

#include "textureshaderclass.h"
#include "spriteclass.h"
#include "TimerClass.h"
#include "fontshaderclass.h"
#include "FontClass.h"
#include "TextClass.h"
#include "fpsclass.h"
#include "modelclass.h"
#include "LightClass.h"
#include "PointLightClass.h"
#include "ShaderManagerClass.h"
#include "FrustumClass.h"
#include "ModelListClass.h"
#include "MovementClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass*);

private:
	bool Render();
	bool UpdateFps();
	bool UpdateRenderCount(int cnt);

	float randF(float mul = 1.0f) {
		return (((float)rand() - (float)rand()) / RAND_MAX )* mul;
	}

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;

	ShaderManagerClass* m_ShaderManager;
	ModelClass* m_Model;

	FontShaderClass* m_FontShader;
	FontClass* m_Font;

	FpsClass* m_Fps;
	TextClass* m_FpsString;
	int m_previousFps;

	LightClass* m_Light;
	int lightCount;
	PointLightClass** m_PointLights;

	TimerClass* m_Timer;

	FrustumClass* m_Frustum;
	PositionClass* m_Position;
	XMMATRIX m_baseViewMatrix;
	TextClass* m_RenderCountString;
	ModelListClass* m_ModelList;
	int* m_ModelRenderTypes;
};

#endif
