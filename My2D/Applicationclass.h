#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#define DEFAULT_WINDOW_WIDTH (800)
#define DEFAULT_WINDOW_HEIGHT (600)

#include "d3dclass.h"
#include "cameraclass.h"

#include "textureshaderclass.h"
#include "spriteclass.h"
#include "TimerClass.h"

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
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TextureShaderClass* m_TextureShader;
	SpriteClass* m_Sprite;
	TimerClass* m_Timer;
};

#endif
