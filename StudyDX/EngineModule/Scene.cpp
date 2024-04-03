#include "Precompiled.h"

Scene Scene::Invalid(MathUtil::InvalidHashName);

bool Scene::operator==(const Scene& InScene) const
{
	return _Hash == InScene.GetHash();
}

bool Scene::operator!=(const Scene& InScene) const
{
	return _Hash != InScene.GetHash();
}

void Scene::AddObject(const ObjectInfo& Ininfo)
{
	_VecInfo.push_back(Ininfo);
}
