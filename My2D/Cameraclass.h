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
	void GetViewMatrix(XMMATRIX& viewMatrix) { viewMatrix = m_viewMatrix; }

	void RenderReflection(float a, float b, float c, float d) { RenderReflection(XMFLOAT4(a, b, c, d)); }
	void RenderReflection(XMFLOAT4 plane);
	XMMATRIX GetReflectionViewMatrix() const { return m_reflectionViewMatrix;}

private:
	Transform* m_transform;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_reflectionViewMatrix;
};

#endif
