#pragma once
class CollisionMngInterface {

public:
	virtual void Init(const SceneMngInterface* InSceneMng) = 0;

	virtual void Update(float InDeltaSeconds) = 0;
	virtual void CheckGroup(ObjectType InLeft, ObjectType InRight) = 0;

};