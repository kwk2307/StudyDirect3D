#pragma once

union COLLIDER_ID {
	struct {
		UINT LeftID;
		UINT RightID;
	};
	LONGLONG llID;
};

struct Interval {
	double min, max;
};

class CollisionMng:
	public CollisionMngInterface
{

public:
	CollisionMng() = default;

public:
	virtual void Init(const SceneMngInterface* InSceneMng) override;

	virtual void Update(float InDeltaSeconds) override;
	
	virtual void CheckGroup(ObjectType InLeft, ObjectType InRight) override;

private:
	void CollisionEvent(ObjectType InLeft, ObjectType InRight);
	bool isCollision(const Object& InLeftObj, const Object& InRightObj);
	Interval project(const std::vector<Vector3>& poly, const Vector3& axis);
	bool overlap(Interval a, Interval b);

private:
	const SceneMngInterface* _SceneMng;

	// Collider끼리 충돌이 되는지 확인하는 검사 테이블 필요
	UINT _CheckTable[(int)ObjectType::End];
	std::map<LONGLONG, bool> m_mColliderTable;

	// Collider 들을 담아 놓은 테이블 필요 
	std::vector<Collider*> _Colliders;
};

