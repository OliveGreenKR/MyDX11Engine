#pragma once
#include "TransformClass.h"

class MovementClass
{
public:
	MovementClass();
	MovementClass(const MovementClass&);
	~MovementClass();

	void SetTransform(TransformClass* transform) { m_transform = transform; }
	void SetFrameTime(float frametime) { m_frameTime = frametime; }
	float GetRotationY() { return m_rotationY; }

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);

private:

	TransformClass* m_transform;

	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_forwardSpeed, m_backwardSpeed;
};

