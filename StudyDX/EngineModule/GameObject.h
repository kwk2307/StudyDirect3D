#pragma once

enum class GameObjectType : UINT32
{
	Normal = 0,
	Gizmo
};

struct Mesh;

class GameObject
{
public:
	GameObject(const std::string& InName) : _Name(InName)
	{
		_Hash = std::hash<std::string>()(_Name);
	}

	~GameObject() {}

public:
	// 종류
	void SetGameObjectType(const GameObjectType& _InGameObjectType) { _GameObjectType = _InGameObjectType; }
	FORCEINLINE bool IsGizmoObject() const { return _GameObjectType == GameObjectType::Gizmo; }

	// 트랜스폼
	TransformComponent& GetTransform() { return _Transform; }
	const TransformComponent& GetTransform() const { return _Transform; }
	void SetParent(GameObject& InGameObject) { _Transform.SetParent(InGameObject.GetTransform()); }

	// 메시
	void SetMesh(std::shared_ptr<Mesh> InMesh) { _Mesh = InMesh; }
	bool HasMesh() const { return _Mesh != nullptr; }
	std::shared_ptr<Mesh> GetMesh() { return _Mesh; }

	// 색상
	void SetColor(const Color& InColor) { _Color = InColor; }
	FORCEINLINE const Color& GetColor() const { return _Color; }

	// 키 관련
	const std::string& GetName() const { return _Name; }
	std::size_t GetHash() const { return _Hash; }

	// 검색 관련
	bool IsValid() const { return _Hash != MathUtil::InvalidHash; }
	FORCEINLINE bool operator==(const GameObject& InGameObject) const;
	FORCEINLINE bool operator!=(const GameObject& InGameObject) const;
	static GameObject Invalid;

	// 렌더링 설정
	bool IsVisible() const { return _IsVisible; }
	void SetVisible(bool InVisible) { _IsVisible = InVisible; }

	void Update(float InDeltaSeconds);
	void Render();
	
public:
	std::function<void()> _OnUpdateFunc;
	std::function<void()> _OnRenderFunc;

private:
	GameObjectType _GameObjectType = GameObjectType::Normal;
	bool _IsVisible = true;
	std::size_t _Hash = MathUtil::InvalidHash;
	std::string _Name;
	
	std::shared_ptr<Mesh> _Mesh;

	TransformComponent _Transform;
	Color _Color;
	//Color _Color = Color::Error;
};

FORCEINLINE bool GameObject::operator==(const GameObject& InGameObject) const
{
	return _Hash == InGameObject.GetHash();
}

FORCEINLINE bool GameObject::operator!=(const GameObject& InGameObject) const
{
	return _Hash == InGameObject.GetHash();
}

