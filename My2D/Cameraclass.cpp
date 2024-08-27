#include "Cameraclass.h"

CameraClass::CameraClass()
{
	m_transform = new Transform;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector, roatationVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position = m_transform->GetPosition();

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt = GetLookAt();

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);
	lookAtVector = XMVector3Normalize(lookAtVector);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	roatationVector = m_transform->GetRotationVector();


	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3Rotate(lookAtVector, roatationVector);
	upVector = XMVector3Rotate(upVector, roatationVector);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void CameraClass::RenderReflection(XMFLOAT4 plane)
{
	XMFLOAT3 up,lookAt;
	XMVECTOR upVector, positionVector, lookAtVector, rotationVector;
	XMVECTOR rfRotationVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;
	XMMATRIX reflectionMatrix;

	// Create the reflection matrix using the plane.
	reflectionMatrix = XMMatrixReflect(XMLoadFloat4(&plane));
	rotationVector = m_transform->GetRotationVector();

	rotationMatrix = XMMatrixRotationQuaternion(rotationVector);
	rotationMatrix =  rotationMatrix * reflectionMatrix;
	rfRotationVector = XMQuaternionRotationMatrix(rotationMatrix);

	up = { 0,1,0 };
	upVector = XMLoadFloat3(&up);
	upVector = XMVector3Rotate(upVector, rfRotationVector);

	m_transform->GetPosition(positionVector);
	positionVector = XMVector3TransformCoord(positionVector, reflectionMatrix);

	lookAt = GetLookAt();

	lookAtVector = XMLoadFloat3(&lookAt);
	lookAtVector = XMVector3Normalize(lookAtVector);
	lookAtVector = XMVector3Rotate(lookAtVector, rfRotationVector);
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);
	
	m_reflectionViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
    return;
}

XMFLOAT3 CameraClass::GetLookAt() const
{
	XMFLOAT3 lookAt;

	if (m_target)
	{
		XMFLOAT3 target, position;
		target = m_target->GetPosition();
		position = m_transform->GetPosition();
		lookAt.x = (target.x - position.x);
		lookAt.y = (target.y - position.y);
		lookAt.z = (target.z - position.z);
	}
	else
	{
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;
	}
	return lookAt;
}


