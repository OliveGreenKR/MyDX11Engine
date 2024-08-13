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
#include "RenderTextureClass.h"
#include "PlaneClass.h"

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
	bool RenderWithShader(ShaderType type, int indexCount, XMMATRIX,XMMATRIX,XMMATRIX);
	bool RenderSceneToTexture(float);


	float randF(float mul = 1.0f) {
		return (((float)rand() - (float)rand()) / RAND_MAX )* mul;
	}
	float randRangeF(float min, float max) {
		return min + (max - min) * rand() / RAND_MAX;
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
	XMMATRIX m_baseViewMatrix;
	TextClass* m_RenderCountString;

	RenderTextureClass* m_RenderTexture;
	DisplayPlaneClass* m_Plane;
};

#endif
