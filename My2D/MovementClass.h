#pragma once
#include "Transform.h"

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float frametime) { m_frameTime = frametime; }
	float GetRotationY() { return m_rotationY; }

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);

private:

	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_forwardSpeed, m_backwardSpeed;
};

