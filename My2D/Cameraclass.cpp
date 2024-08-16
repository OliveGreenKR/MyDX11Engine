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
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

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
    XMFLOAT3 up, position, lookAt;
    XMVECTOR upVector, positionVector, lookAtVector, rotationVector;
    XMMATRIX rotationMatrix;
    XMVECTOR planeNormal, distanceVector, reflectedPositionVector;
    float distance;

    // Setup the position of the camera in the world.
    position = m_transform->GetPosition();
    positionVector = XMLoadFloat3(&position);

    // Calculate the distance between the camera and the reflection plane.
    planeNormal = XMVector3Normalize(XMLoadFloat4(&plane));
    distanceVector = XMPlaneDotCoord(planeNormal, positionVector);
    distance = XMVectorGetX(distanceVector);

    // Calculate the reflected point
    reflectedPositionVector = XMVectorSubtract(positionVector, XMVectorScale(planeNormal, 2.0f * distance));


    // Get the rotation matrix from the transform.
    rotationMatrix = m_transform->GetRotationMatrix();
    rotationVector = m_transform->GetRotationVector();

    // Setup the vector that points upwards.
    up = { 0.0f, 1.0f, 0.0f };
    upVector = XMLoadFloat3(&up);
    upVector = XMVector3Normalize(XMVector3TransformCoord(upVector,rotationMatrix));

    // Setup where the camera is looking by default.
    lookAt = { 0.0f, 0.0f, 1.0f };
    lookAtVector = XMLoadFloat3(&lookAt);
    lookAtVector = XMVector3Normalize(XMVector3TransformCoord(lookAtVector, rotationMatrix));

    // Reflect the rotation vector over the plane
    XMVECTOR reflectedRotationVector = XMVector3Reflect(rotationVector, planeNormal);

    // Create the rotation matrix
    rotationMatrix = XMMatrixRotationQuaternion(reflectedRotationVector);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    lookAtVector = XMVectorAdd(reflectedPositionVector, lookAtVector);

    // Finally create the view matrix from the three updated vectors.
    m_reflectionViewMatrix = XMMatrixLookAtLH(reflectedPositionVector, lookAtVector, upVector);

    return;
}