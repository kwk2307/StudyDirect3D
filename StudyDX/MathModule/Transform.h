#pragma once
struct Transform {
public:
	
	Transform() = default;
	Transform(const Vector3& InPosition) : Position(InPosition) { }
	constexpr Transform(const Vector3& InPosition, const Quaternion& InRotation) : Position(InPosition), Rotation(InRotation) { }
	constexpr Transform(const Vector3& InPosition, const Quaternion& InRotation, const Vector3& InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) { }
	Transform(const Matrix& InMatrix);

public: // 트랜스폼 설정함수
	constexpr void SetPosition(const Vector3& InPosition) { Position = InPosition; }
	constexpr void AddPosition(const Vector3& InDeltaPosition) { Position += InDeltaPosition; }
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

	constexpr void SetRotation(const Rotator& InRotator) { 
		Rotation = Quaternion::CreateFromYawPitchRoll(InRotator.Yaw, InRotator.Pitch, InRotator.Roll); 
	}
	void SetRotation(const Matrix& InMatrix) { 
		Rotation = Quaternion::CreateFromRotationMatrix(InMatrix);
	}
	constexpr void SetRotation(const Quaternion& InQuaternion) { Rotation = InQuaternion; }
	constexpr void SetScale(const Vector3& InScale) { Scale = InScale; }

	constexpr Vector3 GetXAxis() const {
		return DirectX::XMVector3Rotate(Vector3::UnitX, Rotation);
	}
	constexpr Vector3 GetYAxis() const { 
		return DirectX::XMVector3Rotate(Vector3::UnitY, Rotation);
	}
	constexpr Vector3 GetZAxis() const { 
		return DirectX::XMVector3Rotate(Vector3::UnitZ, Rotation);
	}
	constexpr Matrix GetMatrix() const;

	constexpr Vector3 GetPosition() const { return Position; }
	constexpr Quaternion GetRotation() const { return Rotation; }
	constexpr Vector3 GetScale() const { return Scale; }

	// 트랜스폼 변환
	constexpr Transform Inverse() const;
	constexpr Transform LocalToWorld(const Transform& InParentWorldTransform) const;
	constexpr Transform WorldToLocal(const Transform& InParentWorldTransform) const;
	constexpr Vector3 WorldToLocalVector(const Vector3& InWorldVector) const;

private: // 트랜스폼에 관련된 변수
	Vector3 Position;
	Quaternion Rotation;
	Vector3 Scale = Vector3::One;
}; 

constexpr Matrix Transform::GetMatrix() const
{

	
	return Matrix(
		Vector4(GetXAxis() * Scale.x),
		Vector4(GetYAxis() * Scale.y),
		Vector4(GetZAxis() * Scale.z),
		Vector4(Position.x, Position.y, Position.z, 1.f)
	);
}

constexpr Transform Transform::Inverse() const
{

	// 로컬 정보만 남기기 위한 트랜스폼 ( 역행렬 )
	Vector3 reciprocalScale = Vector3::Zero;
	if (!MathUtil::EqualsInTolerance(Scale.x, 0.f)) reciprocalScale.x = 1.f / Scale.x;
	if (!MathUtil::EqualsInTolerance(Scale.y, 0.f)) reciprocalScale.y = 1.f / Scale.y;
	if (!MathUtil::EqualsInTolerance(Scale.z, 0.f)) reciprocalScale.z = 1.f / Scale.z;

	Transform result;
	result.SetScale(reciprocalScale);
	
	Quaternion rot;
	Rotation.Inverse(rot);
	result.SetRotation(rot);

	result.SetPosition(result.GetScale() * (result.GetRotation() * -Position));
	return result;
}


constexpr Transform Transform::LocalToWorld(const Transform& InParentWorldTransform) const
{
	// 현재 트랜스폼 정보가 로컬인 경우
	Transform result;
	result.SetScale(InParentWorldTransform.GetScale() * GetScale());
	
	result.SetRotation(InParentWorldTransform.GetRotation() * GetRotation());

	result.SetPosition(InParentWorldTransform.GetPosition() + InParentWorldTransform.GetRotation() * (InParentWorldTransform.GetScale() * GetPosition()));
	return result;
}

constexpr Transform Transform::WorldToLocal(const Transform& InParentWorldTransform) const
{
	// 현재 트랜스폼 정보가 월드인 경우
	Transform invParent = InParentWorldTransform.Inverse();

	Transform result;
	result.SetScale(invParent.GetScale() * GetScale());
	result.SetRotation(invParent.GetRotation() * GetRotation());
	result.SetPosition(invParent.GetPosition() + (invParent.GetRotation() * (invParent.GetScale() * GetPosition())));
	return result;
}

constexpr Vector3 Transform::WorldToLocalVector(const Vector3& InWorldVector) const
{
	// 현재 트랜스폼을 기준으로 월드 벡터를 로컬 벡터로 변환
	Transform invTransform = Inverse();

	return invTransform.GetPosition() + invTransform.GetScale() * (invTransform.GetRotation() * InWorldVector);
}

}
