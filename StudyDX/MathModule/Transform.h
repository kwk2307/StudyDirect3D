#pragma once
struct Transform {
public:
	
	Transform() = default;
	Transform(const Vector3& InPosition) : Position(InPosition) { }
	constexpr Transform(const Vector3& InPosition, const Quaternion& InRotation) : Position(InPosition), Rotation(InRotation) { }
	constexpr Transform(const Vector3& InPosition, const Quaternion& InRotation, const Vector3& InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) { }
	Transform(const Matrix& InMatrix);

public: // Ʈ������ �����Լ�
	constexpr void SetPosition(const Vector3& InPosition) { Position = InPosition; }
	void AddPosition(const Vector3& InDeltaPosition) { Position += InDeltaPosition; }
	void AddYawRotation(float InDegree)
	{
		Rotator r(Rotation.ToEuler());
		r.Yaw += InDegree;
		r.Clamp();
		Rotation = Quaternion::CreateFromYawPitchRoll(r.Yaw, r.Pitch, r.Roll);
	}
	void AddRollRotation(float InDegree)
	{
		Rotator r(Rotation.ToEuler());
		r.Roll += InDegree;
		r.Clamp();
		Rotation = Quaternion::CreateFromYawPitchRoll(r.Yaw, r.Pitch, r.Roll);
	}
	void AddPitchRotation(float InDegree)
	{
		Rotator r(Rotation.ToEuler());
		r.Pitch += InDegree;
		r.Clamp();
		Rotation = Quaternion::CreateFromYawPitchRoll(r.Yaw, r.Pitch, r.Roll);
	}

	void SetRotation(const Rotator& InRotator) { 
		Rotation = Quaternion::CreateFromYawPitchRoll(InRotator.Yaw, InRotator.Pitch, InRotator.Roll); 
	}
	void SetRotation(const Matrix& InMatrix) { 
		Rotation = Quaternion::CreateFromRotationMatrix(InMatrix);
	}
	constexpr void SetRotation(const Quaternion& InQuaternion) { Rotation = InQuaternion; }
	constexpr void SetScale(const Vector3& InScale) { Scale = InScale; }

	Vector3 GetXAxis() const {
		return DirectX::XMVector3Rotate(Vector3::UnitX, Rotation);
	}
	Vector3 GetYAxis() const { 
		return DirectX::XMVector3Rotate(Vector3::UnitY, Rotation);
	}
	Vector3 GetZAxis() const { 
		return DirectX::XMVector3Rotate(Vector3::UnitZ, Rotation);
	}
	Matrix GetMatrix() const;

	constexpr Vector3 GetPosition() const { return Position; }
	constexpr Quaternion GetRotation() const { return Rotation; }
	constexpr Vector3 GetScale() const { return Scale; }

	// Ʈ������ ��ȯ
	Transform Inverse() const;
	Transform LocalToWorld(const Transform& InParentWorldTransform) const;
	Transform WorldToLocal(const Transform& InParentWorldTransform) const;
	Vector3 WorldToLocalVector(const Vector3& InWorldVector) const;

private: // Ʈ�������� ���õ� ����
	Vector3 Position;
	Quaternion Rotation;
	Vector3 Scale = Vector3::One;
}; 

inline Matrix Transform::GetMatrix() const
{
	return Matrix::CreateScale(Scale) *
		Matrix::CreateFromQuaternion(Rotation) *
		Matrix::CreateTranslation(Position);
}

inline Transform Transform::Inverse() const
{
	Vector3 inverseTranslation = -Position;
	Quaternion inversRotation = DirectX::XMQuaternionInverse(Rotation);
	Vector3 inverseScale = DirectX::XMVectorReciprocal(Scale);

	Transform result;
	result.SetScale(inverseScale);
	result.SetRotation(inversRotation);
	result.SetPosition(result.GetScale() * Vector3(DirectX::XMVector3Rotate(-inverseScale, inversRotation)));
	
	return result;
}

inline Transform Transform::LocalToWorld(const Transform& InParentWorldTransform) const
{
	// ���� Ʈ������ ������ ������ ���
	Transform result;
	result.SetScale(InParentWorldTransform.GetScale() * GetScale());
	
	result.SetRotation(InParentWorldTransform.GetRotation() * GetRotation());

	result.SetPosition(
		InParentWorldTransform.GetPosition() + 
		Vector3(DirectX::XMVector3Rotate((InParentWorldTransform.GetScale() * GetPosition()), InParentWorldTransform.GetRotation())));
	return result;
}

inline Transform Transform::WorldToLocal(const Transform& InParentWorldTransform) const
{
	// ���� Ʈ������ ������ ������ ���
	Transform invParent = InParentWorldTransform.Inverse();

	Transform result;
	result.SetScale(invParent.GetScale() * GetScale());
	result.SetRotation(invParent.GetRotation() * GetRotation());
	result.SetPosition(
		invParent.GetPosition() + 
		Vector3(DirectX::XMVector3Rotate(invParent.GetScale() * GetPosition(), invParent.GetRotation()))
	);
	return result;
}

inline Vector3 Transform::WorldToLocalVector(const Vector3& InWorldVector) const
{
	// ���� Ʈ�������� �������� ���� ���͸� ���� ���ͷ� ��ȯ
	Transform invTransform = Inverse();
	return invTransform.GetPosition() + invTransform.GetScale() * Vector3(DirectX::XMVector3Rotate(InWorldVector, invTransform.GetRotation()));
}

