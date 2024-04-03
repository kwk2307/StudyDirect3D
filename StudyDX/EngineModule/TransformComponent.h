#pragma once
class TransformComponent
{
public:
	TransformComponent() = default;
	TransformComponent(const Vector3& InPos,const Rotator& InRot,const Vector3& InScale):
		_Position(InPos),
		_Rotation(InRot),
		_Scale(InScale){}

	__forceinline Matrix4 GetModelingMatrix() const;


public:
	const Vector3& GetPosition() const{ return _Position; }
	void SetPosition(const Vector3& InPos) { _Position = InPos; }

	const Rotator& GetRotator() const { return _Rotation; }
	void SetRotation(const Rotator& InRot) { _Rotation = InRot; }
	const Vector3& GetScale() const { return _Scale; }

	void AddPosition (const Vector3& InVec){
		_Position += InVec;
	}

	void AddRotator(const Rotator& InRot) {
		_Rotation.AddRotator(InRot);
	}

private:
	Vector3 _Position = Vector3::Zero;
	Rotator _Rotation;
	Vector3 _Scale = Vector3::One;

};

__forceinline Matrix4 TransformComponent::GetModelingMatrix() const {
	Vector3 XAxis, YAxis, ZAxis;
	GetRotator().GetLocalAxes(XAxis, YAxis, ZAxis);
	return Matrix4(
		Vector4(XAxis * GetScale().X, false),
		Vector4(YAxis * GetScale().Y, false),
		Vector4(ZAxis * GetScale().Z, false), 
		Vector4(GetPosition(), true)
	);
}
