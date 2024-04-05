#pragma once
class TransformComponent
{
public:
	TransformComponent() = default;
	TransformComponent(const Transform& InLocalTransform) : _LocalTransform(InLocalTransform)
	{
		UpdateWorld();
	}

public: // 로컬 트랜스폼 관련 함수
	Transform& GetLocalTransform() { return _LocalTransform; }
	void SetLocalTransform(const Transform& InTransform) { _LocalTransform = InTransform; UpdateWorld(); }
	const Transform& GetLocalTransform() const { return _LocalTransform; }

	void SetLocalPosition(const Vector3& InPosition);
	void AddLocalPosition(const Vector3& InDeltaPosition);
	void AddLocalYawRotation(float InDegree);
	void AddLocalRollRotation(float InDegree);
	void AddLocalPitchRotation(float InDegree);
	void SetLocalRotation(const Rotator& InRotator);
	void SetLocalRotation(const Quaternion& InQuaternion);
	void SetLocalRotation(const Matrix& InMatrix);
	void SetLocalScale(const Vector3& InScale);

	Vector3 GetLocalPosition() const { return _LocalTransform.GetPosition(); }
	Rotator GetLocalRotator() const { return _LocalTransform.GetRotation().ToEuler(); }
	Quaternion GetLocalRotation() const { return _LocalTransform.GetRotation(); }
	Vector3 GetLocalScale() const { return _LocalTransform.GetScale(); }

	Vector3 GetLocalX() const { return _LocalTransform.GetXAxis(); }
	Vector3 GetLocalY() const { return _LocalTransform.GetYAxis(); }
	Vector3 GetLocalZ() const { return _LocalTransform.GetZAxis(); }
	Matrix GetLocalMatrix() const { return _LocalTransform.GetMatrix(); }

public: // 월드 트랜스폼 관련 함수
	Transform& GetWorldTransform() { return _WorldTransform; }
	void SetWorldTransform(const Transform& InTransform) { _WorldTransform = InTransform; UpdateLocal(); }
	const Transform& GetWorldTransform() const { return _WorldTransform; }

	void SetWorldPosition(const Vector3& InPosition);
	void AddWorldPosition(const Vector3& InDeltaPosition);
	void AddWorldYawRotation(float InDegree);
	void AddWorldRollRotation(float InDegree);
	void AddWorldPitchRotation(float InDegree);
	void SetWorldRotation(const Rotator& InRotator);
	void SetWorldRotation(const Matrix& InMatrix);
	void SetWorldRotation(const Quaternion& InQuaternion);
	void SetWorldScale(const Vector3& InScale);

	Vector3 GetWorldPosition() const { return _WorldTransform.GetPosition(); }
	Rotator GetWorldRotator() const { return _WorldTransform.GetRotation().ToEuler(); }
	Quaternion GetWorldRotation() const { return _WorldTransform.GetRotation(); }
	Vector3 GetWorldScale() const { return _WorldTransform.GetScale(); }

	Matrix GetWorldMatrix() const { return _WorldTransform.GetMatrix(); }

public: // 계층 구조 관련 함수
	bool SetRoot();
	bool RemoveFromParent();
	TransformComponent& GetRoot();
	bool SetParent(TransformComponent& InTransform);

	bool HasParent() const { return _ParentPtr != nullptr; }
	std::vector<TransformComponent*>& GetChildren() { return _ChildrenPtr; }
	std::vector<TransformComponent*>::const_iterator ChildBegin() const { return _ChildrenPtr.begin(); }
	std::vector<TransformComponent*>::const_iterator ChildEnd() const { return _ChildrenPtr.end(); }

private: // 내부에서만 호출하는 함수
	TransformComponent* GetParentPtr() const { return _ParentPtr; }
	void UpdateLocal();
	void UpdateWorld();
	void UpdateChildrenWorld();

private: // 계층 구조를 위한 변수
	Transform _LocalTransform;
	Transform _WorldTransform;

	TransformComponent* _ParentPtr = nullptr;
	std::vector<TransformComponent*> _ChildrenPtr;
};

inline void TransformComponent::SetLocalPosition(const Vector3& InPos) {
	_LocalTransform.SetPosition(InPos);
	UpdateWorld();
}

inline void TransformComponent::AddLocalPosition(const Vector3& InDeltaPosition)
{
	_LocalTransform.AddPosition(InDeltaPosition);
	UpdateWorld();
}

inline void TransformComponent::AddLocalYawRotation(float InDegree)
{
	_LocalTransform.AddYawRotation(InDegree);
	UpdateWorld();
}

inline void TransformComponent::AddLocalRollRotation(float InDegree)
{
	_LocalTransform.AddRollRotation(InDegree);
	UpdateWorld();
}

inline void TransformComponent::AddLocalPitchRotation(float InDegree)
{
	_LocalTransform.AddPitchRotation(InDegree);
	UpdateWorld();
}

inline void TransformComponent::SetLocalRotation(const Rotator& InRotator)
{
	_LocalTransform.SetRotation(InRotator);
	UpdateWorld();
}

inline void TransformComponent::SetLocalRotation(const Matrix& InMatrix)
{
	_LocalTransform.SetRotation(InMatrix);
	UpdateWorld();
}

inline void TransformComponent::SetLocalRotation(const Quaternion& InQuaternion)
{
	_LocalTransform.SetRotation(InQuaternion);
	UpdateWorld();
}

inline void TransformComponent::SetLocalScale(const Vector3& InScale)
{
	_LocalTransform.SetScale(InScale);
	UpdateWorld();
}

inline void TransformComponent::SetWorldPosition(const Vector3& InPosition)
{
	_WorldTransform.SetPosition(InPosition);
	UpdateLocal();
}

inline void TransformComponent::AddWorldPosition(const Vector3& InDeltaPosition)
{
	_WorldTransform.AddPosition(InDeltaPosition);
	UpdateLocal();
}

inline void TransformComponent::AddWorldYawRotation(float InDegree)
{
	_WorldTransform.AddYawRotation(InDegree);
	UpdateLocal();
}

inline void TransformComponent::AddWorldRollRotation(float InDegree)
{
	_WorldTransform.AddRollRotation(InDegree);
	UpdateLocal();
}

inline void TransformComponent::AddWorldPitchRotation(float InDegree)
{
	_WorldTransform.AddPitchRotation(InDegree);
	UpdateLocal();
}

inline void TransformComponent::SetWorldRotation(const Rotator& InRotator)
{
	_WorldTransform.SetRotation(InRotator);
	UpdateLocal();
}

inline void TransformComponent::SetWorldRotation(const Matrix& InMatrix)
{
	_WorldTransform.SetRotation(InMatrix);
	UpdateLocal();
}

inline void TransformComponent::SetWorldRotation(const Quaternion& InQuaternion)
{
	_WorldTransform.SetRotation(InQuaternion);
	UpdateLocal();
}

inline void TransformComponent::SetWorldScale(const Vector3& InScale)
{
	_WorldTransform.SetScale(InScale);
	UpdateLocal();
}
