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
    void SetPosition(const XMFLOAT3& position) { m_position = position; }
    __forceinline void SetPosition(float x, float y, float z) { m_position = XMFLOAT3(x, y, z); }

    void SetRotation(const XMFLOAT4& rotation) { m_rotation = rotation; }
    __forceinline void SetRotation(const XMVECTOR& rotation) { XMStoreFloat4(&m_rotation, rotation); }
	void SetEulerRotation(float x, float y, float z);

    XMFLOAT3 GetPosition() const { return m_position; }
    XMFLOAT3 GetPosition() { return m_position; }
    XMFLOAT3 GetScale() const { return m_scale; }
    XMFLOAT4 GetRotation() const { return m_rotation; }
	XMVECTOR GetRotationVector() const { return XMLoadFloat4(&m_rotation); }
	inline XMMATRIX GetWorldMatrix() const;

	
public:
	//constexpr Transform Inverse() const;
	//constexpr Transform LocalToWorld(const Transform& ParentWorldTransform) const;
	//constexpr Transform WorldToLocal(const Transform& ParentWorldTransform) const;


private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale = XMFLOAT3(1, 1, 1);
	XMFLOAT4 m_rotation; // Quaternion
};

inline void Transform::SetEulerRotation(float x, float y, float z)
{
	x = XMConvertToRadians(x);
	y = XMConvertToRadians(y);
	z = XMConvertToRadians(z);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(x, y, z);
	XMStoreFloat4(&m_rotation, rotation);

}

inline XMMATRIX Transform::GetWorldMatrix() const
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

