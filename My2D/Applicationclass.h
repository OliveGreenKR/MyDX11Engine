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
#include "multitextureshaderclass.h"
#include "modelclass.h"

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

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;

	MultiTextureShaderClass* m_MultiTextureShader;
	ModelClass* m_Model;

	FontShaderClass* m_FontShader;
	FontClass* m_Font;

	FpsClass* m_Fps;
	TextClass* m_FpsString;
	int m_previousFps;

	

};

#endif
