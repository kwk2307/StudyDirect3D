#include "Precompiled.h"

void CameraObject::SetLookAtRotation(const GameObject& InGameObject, const Vector3& InUp)
{
	SetLookAtRotation(InGameObject.GetTransform().GetWorldPosition(), InUp);
}

void CameraObject::SetLookAtRotation(const Vector3& InTargetPosition, const Vector3& InUp)
{
	_Transform.SetWorldRotation(Quaternion::LookRotation(InTargetPosition, InUp));
}
