#pragma once

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <directxmath.h>

#include "Transform.h"

using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float x, float y, float z) { m_transform->SetPosition(x, y, z); }
	void SetEulerRotation(float roll, float pitch, float yaw) { m_transform->SetEulerRotation(roll, pitch, yaw); }

	XMFLOAT3 GetPosition() { return m_transform->GetPosition(); }
	XMFLOAT4 GetRotation() { return m_transform->GetRotation(); }

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	Transform* m_transform;
	XMMATRIX m_viewMatrix;
};

#endif
