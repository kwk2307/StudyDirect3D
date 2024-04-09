#include "Precompiled.h"

GameObject GameObject::Invalid(MathUtil::InvalidHashName);

void GameObject::Update(float InDeltaSeconds)
{
	_OnUpdateFunc();
}

void GameObject::Render()
{
	_OnRenderFunc();
}
