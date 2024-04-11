#include "Precompiled.h"

const std::size_t GameEngine::RectangleMesh = std::hash<std::string>()("SM_Rectangle");
const std::size_t GameEngine::BoxMesh = std::hash<std::string>()("SM_Box");
const std::size_t GameEngine::SphereMesh = std::hash<std::string>() ("SM_Sphere");

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
	MeshData& rectangle = CreateMesh(GameEngine::RectangleMesh);
	auto& v = rectangle.GetVertices();
	auto& i = rectangle.GetIndices();
	v = MeshData::MakeSquare().GetVertices();
	i = MeshData::MakeSquare().GetIndices();
	rectangle.SetMeshType(MeshType::Normal);

	MeshData& box = CreateMesh(GameEngine::BoxMesh);
	auto& v1 = box.GetVertices();
	auto& i1 = box.GetIndices();
	v1 = MeshData::MakeBox().GetVertices();
	i1 = MeshData::MakeBox().GetIndices();
	box.SetMeshType(MeshType::Normal);

	MeshData& sphere = CreateMesh(GameEngine::SphereMesh);
	auto& v2 = sphere.GetVertices();
	auto& i2 = sphere.GetIndices();
	v2 = MeshData::MakeSphere(1.f, 20, 20).GetVertices();
	i2 = MeshData::MakeSphere(1.f, 20, 20).GetIndices();
	sphere.SetMeshType(MeshType::Normal);

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
	_MainCamera.SetViewportSize(_ScreenSize);
}
