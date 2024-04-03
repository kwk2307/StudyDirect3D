#pragma once
enum class ObjectType
{
	Default,
	Camera,
	Player,
	Block,
	End
};

struct ObjectInfo {
	ObjectType type = ObjectType::Default;
	std::string name;
	TransformComponent transform;

	std::string Mesh;
	std::string Texture;

	bool IsCollision = false;
	bool IsAnimator = false;
};

//왼손 좌표계를 기준으로 저장함
class Object
{
public:
	Object() = delete;
	Object(const ObjectInfo& Info);

public:
	virtual void Update(float InDeltaSeconds);

	const TransformComponent& GetTransform() const { return _Transform; }
	TransformComponent& GetTransform() { return _Transform; }
	bool IsVisible() const { return _IsVisible; }

	const ObjectType& GetType() const { return _ObjectType; }
	const std::string& GetName() const { return _Name; }
	std::size_t GetMeshKey() const { return _Meshkey; }

	std::size_t GetTextureKey() const { return _Texturekey; }

	// 소유권을 넘겨주는 것이 아닌 잠시 빌려주는 것(?)
	// Collider 객체의 cosnt 멤버 함수만을 호출할 수 있음
	// const Object 객체 일때만 이 함수를 호출 가능 sd
	const Collider* GetCollider() const { return _Collider.get(); }
	Collider* GetCollider() { return _Collider.get(); }

	Animator* GetAnimator() { return _Animator.get(); }

	bool HasMesh() const { return _Meshkey != MathUtil::InvalidHash; }
	
	void SetMeshKey(size_t InKey) { _Meshkey = InKey; }
	void SetTextureKey(size_t InKey) { _Texturekey = InKey; }

	bool GetState() const { return _IsDead; }

public:

	virtual void OnCollision(const Collider& InOther);

	virtual void BeginCollision(const Collider& InOther);

	virtual void EndCollision(const Collider& InOther);

private:
	TransformComponent _Transform;
	ObjectType _ObjectType;
	
	std::unique_ptr<Collider> _Collider = nullptr;
	std::unique_ptr<Animator> _Animator = nullptr;

	bool _IsVisible = true;
	std::size_t _Hash = MathUtil::InvalidHash;
	std::string _Name;

	std::size_t _Meshkey =  MathUtil::InvalidHash;
	std::size_t _Texturekey = MathUtil::InvalidHash;

	bool _IsDead = false;

private:
	//void DrawMesh(const Mesh& InMesh, const )
};
