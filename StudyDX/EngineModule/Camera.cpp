#include "Precompiled.h"

Camera::Camera(const ObjectInfo& Info):
	Object(Info)
{

}


Matrix4 Camera::GetViewMatrix() const
{

	switch (_Mode)
	{
	case CameraMode::Orthographic:


		break;
	case CameraMode::Perspective:
		break;
	default:
		break;
	}
	// ViewMatrix 만들어야함
	// Scale을 뺀 Rotation과 Transform 으로 만든 행렬의
	// 역행렬
	Vector3 pos = GetTransform().GetPosition();
	Vector3 viewX, viewY, viewZ;
	GetTransform().GetRotator().GetLocalAxes(viewX, viewY, viewZ);

	return Matrix4(
		Vector4(Vector3(viewX.X, viewY.X, viewZ.X), false), 
		Vector4(Vector3(viewX.Y, viewY.Y, viewZ.Y), false),
		Vector4(Vector3(viewX.Z, viewY.Z, viewZ.Z), false),
		Vector4(-viewX.Dot(pos), -viewY.Dot(pos), -viewZ.Dot(pos),1.0f)
	);
}

Matrix4 Camera::GetViewMatrixRotationOnly() const
{
	return Matrix4();
}

Matrix4 Camera::GetProjectionMatrix() const
{
	switch (_Mode)
	{
	case CameraMode::Orthographic:
		return Matrix4(
			Vector4(2.f / _ViewportSize.X, 0.f, 0.f, 0.f),
			Vector4(0.f, 2.f / _ViewportSize.Y, 0.f, 0.f),
			Vector4(0.f, 0.f, 1 / _FarZ - _NearZ, 1 / _NearZ - _FarZ),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);
		break;

	case CameraMode::Perspective:
		return Matrix4();
		break;
	default:
		return Matrix4();
		break;
	}
}

Matrix4 Camera::GetProjectionViewMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}
