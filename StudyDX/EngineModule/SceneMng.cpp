#include "Precompiled.h"

struct SceneCompare
{
	bool operator()(const std::unique_ptr<Scene>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

bool SceneMng::Init(const CollisionMngInterface* InCollisionMng)
{
	_CollisionMng = InCollisionMng;

	std::unique_ptr<Scene> startScene = std::make_unique<Scene>("StartScene");

	Mesh& PlayerMesh = CreateMesh(std::hash<std::string>()("M_PlayerPlane"));

	std::vector<size_t> vec_Indices = { 0,1,2,0,2,3 };
	PlayerMesh.GetIndices().assign(vec_Indices.begin(), vec_Indices.end());

	std::vector<Vector3> vec_Vertices = {
		Vector3(-32.f,-22.f,0.f),
		Vector3(32.f,-22.f,0.f),
		Vector3(32.f,22.f,0.f),
		Vector3(-32.f,22.f,0.f)
	};
	PlayerMesh.GetVertices().assign(vec_Vertices.begin(), vec_Vertices.end());
	// 가로 64 세로 44
	std::vector<Vector2> vec_UV = {
		Vector2(0.f,1.f),
		Vector2(1.f,1.f),
		Vector2(1.f,0.f),
		Vector2(0.f,0.f),
	};
	PlayerMesh.GetUVs().assign(vec_UV.begin(), vec_UV.end());

	PlayerMesh.CalculateBounds();

	Mesh& BlockMesh = CreateMesh(std::hash<std::string>()("M_Block"));

	// 272/79  320/127 
 	
	// 480/176

	std::vector<size_t> vec_Indices2 = { 0,1,2,0,2,3 };
	BlockMesh.GetIndices().assign(vec_Indices2.begin(), vec_Indices2.end());

	std::vector<Vector3> vec_Vertices2 = {
		Vector3(-120.f,0.f,0.f),
		Vector3(120.f,0.f,0.f),
		Vector3(120.f,40.f,0.f),
		Vector3(-120.f,40.f,0.f)
	};
	BlockMesh.GetVertices().assign(vec_Vertices2.begin(), vec_Vertices2.end());

	std::vector<Vector2> vec_UV2 = {
		Vector2(0.f,1.f),
		Vector2(4.f,1.f),
		Vector2(4.f,0.f),
		Vector2(0.f,0.f),
	};
	BlockMesh.GetUVs().assign(vec_UV2.begin(), vec_UV2.end());

	BlockMesh.CalculateBounds();

	CreateTexture(std::hash<std::string>()("Warrior_Idle_1"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_1.png");
	CreateTexture(std::hash<std::string>()("Warrior_Idle_2"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_2.png");
	CreateTexture(std::hash<std::string>()("Warrior_Idle_3"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_3.png");
	CreateTexture(std::hash<std::string>()("Warrior_Idle_4"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_4.png");
	CreateTexture(std::hash<std::string>()("Warrior_Idle_5"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_5.png");
	CreateTexture(std::hash<std::string>()("Warrior_Idle_6"), "\\Warrior\\Individual Sprite\\idle\\Warrior_Idle_6.png");

	CreateTexture(std::hash<std::string>()("Warrior_Run_1"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_1.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_2"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_2.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_3"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_3.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_4"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_4.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_5"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_5.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_6"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_6.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_7"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_7.png");
	CreateTexture(std::hash<std::string>()("Warrior_Run_8"), "\\Warrior\\Individual Sprite\\Run\\Warrior_Run_8.png");
	
	CreateTexture(std::hash<std::string>()("Warrior_Jump_1"), "\\Warrior\\Individual Sprite\\Jump\\Warrior_Jump_1.png");
	CreateTexture(std::hash<std::string>()("Warrior_Jump_2"), "\\Warrior\\Individual Sprite\\Jump\\Warrior_Jump_2.png");
	CreateTexture(std::hash<std::string>()("Warrior_Jump_3"), "\\Warrior\\Individual Sprite\\Jump\\Warrior_Jump_3.png");

	CreateTexture(std::hash<std::string>()("Warrior_Fall_1"), "\\Warrior\\Individual Sprite\\Fall\\Warrior_Fall_1.png");
	CreateTexture(std::hash<std::string>()("Warrior_Fall_2"), "\\Warrior\\Individual Sprite\\Fall\\Warrior_Fall_2.png");
	CreateTexture(std::hash<std::string>()("Warrior_Fall_3"), "\\Warrior\\Individual Sprite\\Fall\\Warrior_Fall_3.png");

	CreateTexture(std::hash<std::string>()("Block"), "\\Country-village_asset_pack\\1_Tileset & props\\country village tileset.png", std::make_pair(274, 79), std::make_pair(40, 40));

	Texture& BlockTexture = CreateTexture(std::hash<std::string>()("T_Block"), Color::Green);

	ObjectInfo Player;
	Player.name = "player";
	Player.type = ObjectType::Player;
	Player.transform = TransformComponent(Vector3(0.f, 0.f, 0.f), Rotator(0.f, 0.f, 0.f), Vector3(1, 1, 1));
	Player.Mesh = "M_PlayerPlane";
	Player.IsCollision = true;
	Player.IsAnimator = true;
	
	startScene.get()->AddObject(Player);

	ObjectInfo Camera;
	Camera.name = "camera";
	Camera.type = ObjectType::Camera;
	startScene.get()->AddObject(Camera);

	ObjectInfo Block;
	Block.name = "block";
	Block.type = ObjectType::Block;
	Block.transform = TransformComponent(Vector3(0, -150, 0), Rotator(0.f, 0.f, 0.f), Vector3(3, 3, 1));
	Block.Mesh = "M_Block";
	Block.Texture = "Block";
	Block.IsCollision = true;

	startScene.get()->AddObject(Block);

	_Scenes.push_back(std::move(startScene));


	return LoadScene("StartScene");
}

Mesh& SceneMng::CreateMesh(const std::size_t& Inkey)
{
	auto meshPtr = std::make_unique<Mesh>();
	_Meshes.insert({ Inkey,std::move(meshPtr) });

	return *_Meshes.at(Inkey).get();
}

Mesh& SceneMng::CreateMesh(std::string InName, std::vector<Vector3> InVertices, std::vector<size_t> Indices, std::vector<Vector2> InUVs)
{
	auto meshPtr = std::make_unique<Mesh>(InName,InVertices, Indices, InUVs);

	std::size_t key = std::hash<std::string>()(InName);

	_Meshes.insert({ key,std::move(meshPtr) });

	return *_Meshes.at(key).get();

}

Texture& SceneMng::CreateTexture(const std::size_t& Inkey, const std::string& InAddress)
{
	auto meshPtr = std::make_unique<Texture>(InAddress);
	_Textures.insert({ Inkey,std::move(meshPtr) });

	return *_Textures.at(Inkey).get();

}

Texture& SceneMng::CreateTexture(const std::size_t& Inkey, const std::string& InAddress, std::pair<UINT32, UINT32> start, std::pair<UINT32, UINT32> size)
{
	auto meshPtr = std::make_unique<Texture>(InAddress,start,size);
	_Textures.insert({ Inkey,std::move(meshPtr) });

	return *_Textures.at(Inkey).get();

}

Texture& SceneMng::CreateTexture(const std::size_t& Inkey, const Color& Incolor)
{
	auto meshPtr = std::make_unique<Texture>(Incolor);
	_Textures.insert({ Inkey,std::move(meshPtr) });

	return *_Textures.at(Inkey).get();

}

void SceneMng::OnScreenResize(const ScreenPoint& InScreenSize)
{
	_ScreenSize = InScreenSize;
	_MainCamera->SetViewportSize(InScreenSize);
}

bool SceneMng::LoadScene(std::string SceneName)
{
	std::size_t targetHash = std::hash<std::string>()(SceneName);
	const auto it = std::lower_bound(_Scenes.begin(), _Scenes.end(), targetHash, SceneCompare());

	// 소유권은 넘기지 않고 const 형태의 참조로 가져온다. 
	const Scene& Scene = (it != _Scenes.end()) ? *(*it).get() : Scene::Invalid;

	if (Scene == Scene::Invalid) {
		return false;
	}

	_CurrentSceneName = SceneName;
	
	// 오브젝트 목록 초기화  
	for (UINT Type = 0; Type < (UINT)ObjectType::End; ++Type) {
		_Objects[Type].clear();
	}
	

	for (auto it = Scene.GetInfo().begin(); it != Scene.GetInfo().end(); ++it) {
		const ObjectInfo& objectinfo = (*it);
		switch (objectinfo.type)
		{
		case ObjectType::Default: 
		{
			_Objects[(UINT)ObjectType::Default].push_back(std::make_shared<Object>(objectinfo));
			break;
		}
		case ObjectType::Block:
		{
			_Objects[(UINT)ObjectType::Block].push_back(std::make_shared<Object>(objectinfo));
			break;
		}
		case ObjectType::Camera:
		{
			std::shared_ptr<Camera> CameraPtr = std::make_shared<Camera>(objectinfo);
			_MainCamera = CameraPtr.get();
			_Objects[(UINT)ObjectType::Camera].push_back(CameraPtr);
			break;
		}
		case ObjectType::Player:
		{
			std::shared_ptr<Player> PlayerPtr = std::make_shared<Player>(objectinfo);
			_MainPlayer = PlayerPtr.get();
			_Objects[(UINT)ObjectType::Player].push_back(PlayerPtr);
			break;
		}
		default:
			break;
		}
	}
	return true;
}

void SceneMng::Update(float InDeltaSeconds)
{
	for (UINT Type = 0; Type < (UINT)ObjectType::End; ++Type) {
		for (auto it = GetCurrentScene(Type).begin();
			it != GetCurrentScene(Type).end(); ++it) {
			Object& object = *(*it);

			object.Update(InDeltaSeconds);
		}
	}
}

