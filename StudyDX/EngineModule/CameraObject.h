#pragma once
class CameraObject
{
public:
	CameraObject() = default;
	~CameraObject() { }

public:
	// 트랜스폼
	TransformComponent& GetTransform() { return _Transform; }
	const TransformComponent& GetTransform() const { return _Transform; }
	void SetParent(GameObject& InGameObject) { _Transform.SetParent(InGameObject.GetTransform()); }

	// 카메라 값을 가져오는 함수
	float GetFOV() const { return _FOV; }
	float GetNearZ() const { return _NearZ; }
	float GetFarZ() const { return _FarZ; }
	const ScreenPoint& GetViewportSize() const { return _ViewportSize; }

	// 카메라 값을 설정하는 함수
	void SetLookAtRotation(const GameObject& InGameObject, const Vector3& InUp = Vector3::UnitY);
	void SetLookAtRotation(const Vector3& InTargetPosition, const Vector3& InUp = Vector3::UnitY);
	void SetFOV(float InFOV) { _FOV = InFOV; }
	void SetNearZ(float InNearZ) { _NearZ = InNearZ; }
	void SetFarZ(float InFarZ) { _FarZ = InFarZ; }
	void SetViewportSize(const ScreenPoint& InViewportSize) { _ViewportSize = InViewportSize; }

	// 행렬 생성
	void GetViewAxes(Vector3& OutViewX, Vector3& OutViewY, Vector3& OutViewZ) const;
	Matrix GetViewMatrixRotationOnly() const;
	Matrix GetPerspectiveMatrix() const;
	Matrix GetPerspectiveViewMatrix() const;
	Matrix GetViewMatrix() const;

private:
	TransformComponent _Transform;

	bool _UsePerspectiveProjection = false;

	float _FOV = 60.f;
	float _NearZ = 5.5f;
	float _FarZ = 5000.f;
	ScreenPoint _ViewportSize;
};

inline void CameraObject::GetViewAxes(Vector3& OutViewX, Vector3& OutViewY, Vector3& OutViewZ) const
{
	OutViewZ = _Transform.GetLocalZ();
	OutViewY = _Transform.GetLocalY();
	OutViewX = _Transform.GetLocalX();
}

inline Matrix CameraObject::GetViewMatrixRotationOnly() const
{
	return Matrix();
}

inline Matrix CameraObject::GetPerspectiveMatrix() const
{
	if (_UsePerspectiveProjection) {
		return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(_FOV), _ViewportSize.AspectRatio(), _NearZ, _FarZ);
	}
	else {
		return DirectX::XMMatrixOrthographicOffCenterLH(-_ViewportSize.AspectRatio(), _ViewportSize.AspectRatio(), -1.0f, 1.0f, _NearZ, _FarZ);
	}
}

inline Matrix CameraObject::GetPerspectiveViewMatrix() const
{
	return Matrix();
}

inline Matrix CameraObject::GetViewMatrix() const
{
	return
		Matrix::CreateTranslation(_Transform.GetLocalPosition()) *
		Matrix::CreateFromQuaternion(_Transform.GetLocalRotation());
}
