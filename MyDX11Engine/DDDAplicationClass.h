#pragma once

#ifndef _DDDAPPLICATIONCLASS_H_
#define _DDDAPPLICATIONCLASS_H_

#define DEFAULT_WINDOW_WIDTH (800)
#define DEFAULT_WINDOW_HEIGHT (600)

#include "Applicationclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "LightShaderclass.h"
#include "LightClass.h"
#include <vector>

class DDDApplicationClass : ApplicationClass
{
public:
	DDDApplicationClass();
	DDDApplicationClass(const DDDApplicationClass&);
	~DDDApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

protected:
	bool Render(float) override;

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	vector<LightClass> m_Lights;
};

#endif
