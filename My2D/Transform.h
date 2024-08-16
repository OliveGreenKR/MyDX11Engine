#pragma once
#include <math.h>
#include <DirectXMath.h>
#include <stdexcept>
using namespace std;
using namespace DirectX;

struct Transform
{
public:
	Transform() : m_position() { XMStoreFloat4(&m_rotation, XMQuaternionIdentity()); }
	Transform(const XMFLOAT3& position) : m_position(position) { XMStoreFloat4(&m_rotation, XMQuaternionIdentity()); }
	Transform(const XMFLOAT3& position, const XMFLOAT4& rotation) : m_position(position), m_rotation(rotation) {}
	Transform(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale) : m_position(position), m_rotation(rotation), m_scale(scale) {}

public:
	__forceinline void SetPosition(const XMFLOAT3& position) { m_position = position; }
    __forceinline void SetPosition(float x, float y, float z) { m_position = XMFLOAT3(x, y, z); }

	__forceinline void AddPosition(const XMFLOAT3& position) { m_position.x += position.x; m_position.y += position.y; m_position.z += position.z; }
	__forceinline void AddPosition(float x, float y, float z) { m_position.x += x; m_position.y += y; m_position.z += z; }

	__forceinline void SetRotation(const XMFLOAT4& rotation) { m_rotation = rotation; }
    __forceinline void SetRotation(const XMVECTOR& rotation) { XMStoreFloat4(&m_rotation, rotation); }
	__forceinline void SetEulerRotation(float x, float y, float z);

	__forceinline void SetScale(const XMFLOAT3& scale) { m_scale = scale; }
	__forceinline void SetScale(float x, float y, float z) { m_scale = XMFLOAT3(x, y, z); }

    XMFLOAT3 GetPosition() const { return m_position; }
    XMFLOAT3 GetPosition() { return m_position; }
    XMFLOAT3 GetScale() const { return m_scale; }
    XMFLOAT4 GetRotation() const { return m_rotation; }
	XMVECTOR GetRotationVector() const { return XMLoadFloat4(&m_rotation); }
	inline XMMATRIX GetModelingMatrix() const;
	inline XMMATRIX GetRotationMatrix() const { return XMMatrixRotationQuaternion(XMLoadFloat4(&m_rotation)); }
	inline XMMATRIX GetTranslationMatrix() const { return XMMatrixTranslationFromVector(XMLoadFloat3(&m_position)); }
	inline XMMATRIX GetScaleMatrix() const { return XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)); }


	
public:
	//constexpr Transform Inverse() const;
	//constexpr Transform LocalToWorld(const Transform& ParentWorldTransform) const;
	//constexpr Transform WorldToLocal(const Transform& ParentWorldTransform) const;

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale = XMFLOAT3(1, 1, 1);
	XMFLOAT4 m_rotation; // Quaternion
};

inline void Transform::SetEulerRotation(float xDegree, float yDegree, float zDegree)
{
	xDegree = XMConvertToRadians(xDegree);
	yDegree = XMConvertToRadians(yDegree);
	zDegree = XMConvertToRadians(zDegree);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(xDegree, yDegree, zDegree);
	XMStoreFloat4(&m_rotation, rotation);

}

inline XMMATRIX Transform::GetModelingMatrix() const
{
	XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
	XMMATRIX rotation = XMMatrixRotationQuaternion(XMLoadFloat4(&m_rotation));
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position));
	return scale * rotation * translation;
}

//inline constexpr Transform Transform::Inverse() const
//{
//	throw std::logic_error("Inverse function is not defined.");
//}
//
//inline constexpr Transform Transform::LocalToWorld(const Transform& ParentWorldTransform) const
//{
//	throw std::logic_error("LocalToWorld function is not defined.");
//}
//
//inline constexpr Transform Transform::WorldToLocal(const Transform& ParentWorldTransform) const
//{
//	throw std::logic_error("WorldToLocal function is not defined.");
//}

