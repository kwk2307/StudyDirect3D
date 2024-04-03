#include "Precompiled.h"

UINT Collider::_NextID = 0;

Collider::Collider(Object& InOwner):
	_Owner(InOwner),
	_ID(_NextID++)
{
}

Collider::~Collider()
{

}

void Collider::OnCollision(const Collider& InOther)
{
	_Owner.OnCollision(InOther);
}

void Collider::BeginCollision(const Collider& InOther)
{
	_Owner.BeginCollision(InOther);
}

void Collider::EndCollision(const Collider& InOther)
{
	_Owner.EndCollision(InOther);
}

