#include "Precompiled.h"


Object::Object(const ObjectInfo& Info):
	_ObjectType(Info.type),
	_Transform(Info.transform)
{
	if (Info.Mesh != "") {
		_Meshkey = std::hash<std::string>()(Info.Mesh);
	}

	if (Info.Texture != "") {
		_Texturekey = std::hash<std::string>()(Info.Texture);
	}

	if (Info.IsCollision) {
		_Collider = std::make_unique<Collider>(*this);

	}

	if (Info.IsAnimator) {
		_Animator = std::make_unique<Animator>();
	}

}

void Object::Update(float InDeltaSeconds)
{

}

void Object::OnCollision(const Collider& InOther)
{
}

void Object::BeginCollision(const Collider& InOther)
{
}

void Object::EndCollision(const Collider& InOther)
{
}

