#pragma once

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
	void SetMesh(const std::size_t& InMeshKey) { _MeshKey = InMeshKey; }
	bool HasMesh() const { return _MeshKey != MathUtil::InvalidHash; }
	FORCEINLINE std::size_t GetMeshKey() const { return _MeshKey; }

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

private:
	GameObjectType _GameObjectType = GameObjectType::Normal;
	bool _IsVisible = true;
	std::size_t _Hash = MathUtil::InvalidHash;
	std::string _Name;
	std::size_t _MeshKey = MathUtil::InvalidHash;
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

