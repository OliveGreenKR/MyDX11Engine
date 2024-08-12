#pragma once
#include "Transform.h"

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetFrameTime(float frametime) { m_frameTime = frametime; }
	float GetRotationY() { return m_rotationY; }
	const Transform* GetTransform() { return m_transform; }

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);

private:

	Transform* m_transform;

	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_forwardSpeed, m_backwardSpeed;
};

