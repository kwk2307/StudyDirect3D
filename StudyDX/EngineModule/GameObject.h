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
	// ����
	void SetGameObjectType(const GameObjectType& _InGameObjectType) { _GameObjectType = _InGameObjectType; }
	FORCEINLINE bool IsGizmoObject() const { return _GameObjectType == GameObjectType::Gizmo; }

	// Ʈ������
	TransformComponent& GetTransform() { return _Transform; }
	const TransformComponent& GetTransform() const { return _Transform; }
	void SetParent(GameObject& InGameObject) { _Transform.SetParent(InGameObject.GetTransform()); }

	// �޽�
	void SetMesh(const std::size_t& InMeshKey) { _MeshKey = InMeshKey; }
	bool HasMesh() const { return _MeshKey != MathUtil::InvalidHash; }
	FORCEINLINE std::size_t GetMeshKey() const { return _MeshKey; }

	// ����
	void SetColor(const Color& InColor) { _Color = InColor; }
	FORCEINLINE const Color& GetColor() const { return _Color; }

	// Ű ����
	const std::string& GetName() const { return _Name; }
	std::size_t GetHash() const { return _Hash; }

	// �˻� ����
	bool IsValid() const { return _Hash != MathUtil::InvalidHash; }
	FORCEINLINE bool operator==(const GameObject& InGameObject) const;
	FORCEINLINE bool operator!=(const GameObject& InGameObject) const;
	static GameObject Invalid;

	// ������ ����
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

