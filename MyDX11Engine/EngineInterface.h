#pragma once
#ifndef _ENGINEINTERFACE_H_
#define _ENGINEINTERFACE_H_


__interface IEngine
{
    enum EngineType
    {
        ENGINE_3D = 0,
        ENGINE_2D,
    };

public:
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

};

#endif