#pragma once

class Object;

class Collider
{
public:
	Collider(Object& InOwner);
	~Collider();
public:
	const Object& GetOwner() const { return _Owner; }

	UINT GetID() const { return _ID; }

	void OnCollision(const Collider& InOther);
	void BeginCollision(const Collider& InOther);
	void EndCollision(const Collider& InOther);

private:

	static UINT _NextID;
	UINT _ID;
	Object& _Owner;
};


