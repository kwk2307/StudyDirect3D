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
			// 중복된 키 발생. 무시.
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

std::vector<std::shared_ptr<MeshData>>& GameEngine::CreateMesh(const std::size_t& InKey)
{
	auto iter = _Meshes.find(InKey);
	if (iter == _Meshes.end()) {
		std::vector<std::shared_ptr<MeshData>> vec;
		_Meshes.insert({ InKey, vec });
	}

	return _Meshes.at(InKey);
}

std::vector<std::shared_ptr<MeshData>>& GameEngine::CreateMesh(const std::size_t& InKey, const MeshData& InMesh)
{
	auto iter = _Meshes.find(InKey);
	if (iter == _Meshes.end()) {
		std::vector<std::shared_ptr<MeshData>> vec;
		vec.push_back(std::make_shared<MeshData>(InMesh));
		
		_Meshes.insert({ InKey, vec });
	}
	else {
		iter->second.push_back(std::make_shared<MeshData>(InMesh));
	}
	return _Meshes.at(InKey);
}

std::vector<std::shared_ptr<MeshData>>& GameEngine::CreateMesh(const std::size_t& InKey, std::string InBasePath, std::string Infilename)
{
	// 경로 안에 파일이름 찾아서 넣는걸로
	// 만들어야 할듯 ?
	std::vector<MeshData> meshes = ReadFromFile(InBasePath, Infilename);
	for (const auto& mesh : meshes) {
		auto iter = _Meshes.find(InKey);
		if (iter == _Meshes.end()) {
			std::vector<std::shared_ptr<MeshData>> vec;
			vec.push_back(std::make_shared<MeshData>(mesh));

			_Meshes.insert({ InKey, vec });
		}
		else {
			iter->second.push_back(std::make_shared<MeshData>(mesh));
		}
	}
	return _Meshes.at(InKey);
}

std::vector<MeshData> GameEngine::ReadFromFile(std::string InBasePath, std::string Infilename)
{
	using namespace DirectX;

	ModelLoader modelLoader;
	modelLoader.Load(InBasePath, Infilename);
	std::vector<MeshData>& meshes = modelLoader.meshes;

	// Normalize vertices
	Vector3 vmin(1000, 1000, 1000);
	Vector3 vmax(-1000, -1000, -1000);
	for (auto& mesh : meshes) {
		for (auto& v : mesh._Vertices) {
			vmin.x = XMMin(vmin.x, v.position.x);
			vmin.y = XMMin(vmin.y, v.position.y);
			vmin.z = XMMin(vmin.z, v.position.z);
			vmax.x = XMMax(vmax.x, v.position.x);
			vmax.y = XMMax(vmax.y, v.position.y);
			vmax.z = XMMax(vmax.z, v.position.z);
		}
	}

	float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
	float dl = XMMax(XMMax(dx, dy), dz);
	float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
		cz = (vmax.z + vmin.z) * 0.5f;

	for (auto& mesh : meshes) {
		for (auto& v : mesh._Vertices) {
			v.position.x = (v.position.x - cx) / dl;
			v.position.y = (v.position.y - cy) / dl;
			v.position.z = (v.position.z - cz) / dl;
		}
	}

	return meshes;
}

std::shared_ptr<TextureData>& GameEngine::CreateTexture(const std::size_t& InKey, std::string InBasePath, std::string Infilename)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

bool GameEngine::LoadResources()
{
	CreateMesh(GameEngine::BoxMesh).push_back(std::make_shared<MeshData>(GeometryGenerator::MakeBox()));

	std::vector<MeshData> meshes =  
		ReadFromFile(PathMng::GetInstance()->GetContentPath()+"zeldaPosed001\\", "zeldaPosed001.fbx");
	for (const auto& mesh : meshes) {
		CreateMesh(std::hash<std::string>() ("zelda"), mesh);
	}

	return true;
}

bool GameEngine::Init()
{
	// 이미 초기화되어 있으면 초기화 진행하지 않음.
	if (_IsInitialized)
	{
		return true;
	}

	// 화면 크기가 올바로 설정되어 있는지 확인
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
}

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	// 화면 크기의 설정
	_ScreenSize = InScreenSize;
	_MainCamera.SetViewportSize(_ScreenSize);
}
