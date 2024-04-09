#include "Precompiled.h"

const std::size_t GameEngine::PlaneMesh = std::hash<std::string>()("SM_Plane");

struct GameObjectCompare
{
	bool operator()(const std::unique_ptr<GameObject>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

GameObject& GameEngine::GetGameObject(const std::string& InName)
{
	std::size_t targetHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), targetHash, GameObjectCompare());

	return (it != _Scene.end()) ? *(*it).get() : GameObject::Invalid;
}

GameObject& GameEngine::CreateNewGameObject(const std::string& InName)
{
	std::size_t inHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), inHash, GameObjectCompare());

	auto newGameObject = std::make_unique<GameObject>(InName);
	if (it != _Scene.end())
	{
		std::size_t targetHash = (*it)->GetHash();
		if (targetHash == inHash)
		{
			// �ߺ��� Ű �߻�. ����.
			assert(false);
			return GameObject::Invalid;
		}
		else if (targetHash < inHash)
		{
			_Scene.insert(it + 1, std::move(newGameObject));
		}
		else
		{
			_Scene.insert(it, std::move(newGameObject));
		}
	}
	else
	{
		_Scene.push_back(std::move(newGameObject));
	}

	return GetGameObject(InName);
}

MeshData& GameEngine::CreateMesh(const std::size_t& InKey)
{
	auto meshPtr = std::make_unique<MeshData>();
	_Meshes.insert({ InKey, std::move(meshPtr) });
	return *_Meshes.at(InKey).get();
}

bool GameEngine::LoadResources()
{
	MeshData& plane = CreateMesh(GameEngine::PlaneMesh);
	
	auto& v = plane.GetVertices();
	auto& i = plane.GetIndices();
	
	v = MeshData::MakeSquare().GetVertices();
	i = MeshData::MakeSquare().GetIndices();
	
	plane.SetMeshType(MeshType::Normal);

	return true;
}

bool GameEngine::Init()
{
	// �̹� �ʱ�ȭ�Ǿ� ������ �ʱ�ȭ �������� ����.
	if (_IsInitialized)
	{
		return true;
	}

	// ȭ�� ũ�Ⱑ �ùٷ� �����Ǿ� �ִ��� Ȯ��
	if (_ScreenSize.HasZero())
	{
		return false;
	}

	//if (!_InputManager.IsInputReady())
	//{
	//	return false;
	//}

	if (!LoadResources())
	{
		return false;
	}

	_IsInitialized = true;
	return _IsInitialized;
	
	return _IsInitialized;
}

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	// ȭ�� ũ���� ����
	_ScreenSize = InScreenSize;
	//_MainCamera.SetViewportSize(_ScreenSize);
}
