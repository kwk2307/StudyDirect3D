#pragma once
class SceneMng :
	public SceneMngInterface
{

public:
	SceneMng() = default;
public:
	virtual bool Init(const CollisionMngInterface* InCollisionMng) override;

	const std::string& GetCurrentSceneName() const { return _CurrentSceneName; }

	virtual const std::vector<std::shared_ptr<Object>>& GetCurrentScene(UINT InType) const override{ 
		return _Objects[InType];
	}

	Mesh& CreateMesh(const std::size_t& Inkey);
	Mesh& CreateMesh(std::string InName, std::vector<Vector3> InVertices, std::vector<size_t> Indices, std::vector<Vector2> InUVs);

	Texture& CreateTexture(const std::size_t& Inkey, const std::string& InAddress);
	Texture& CreateTexture(const std::size_t& Inkey, const std::string& InAddress, std::pair<UINT32, UINT32> start, std::pair<UINT32, UINT32> size);
	Texture& CreateTexture(const std::size_t& Inkey, const Color& Incolor);

	virtual Mesh& GetMesh(const std::size_t& InMeshKey) const override { return *_Meshes.at(InMeshKey).get(); }
	virtual Texture& GetTexture(const std::size_t InTextureKey) const override { return *_Textures.at(InTextureKey).get(); }

	virtual void OnScreenResize(const ScreenPoint& InScreenSize) override;

	virtual Camera& GetCamera() override { return *_MainCamera; }
	virtual Player& GetPlayer() override { return *_MainPlayer; }

	bool LoadScene(std::string SceneName);

	virtual void Update(float InDeltaSeconds) override;

private:
	// 여러개의 씬을 담아 둘 벡터
	std::vector<std::unique_ptr<Scene>> _Scenes;

private:
	// 오브젝트를 로드해서 넣어놓을 벡터 
	std::vector<std::shared_ptr<Object>> _Objects[(UINT)ObjectType::End];
	 
	// 메쉬를 담어둘 unordered_map 
	std::unordered_map<std::size_t, std::unique_ptr<Mesh>> _Meshes;
	
	// 텍스쳐를 담아둘 unordered_map 
	std::unordered_map<std::size_t, std::unique_ptr<Texture>> _Textures;

	std::string _CurrentSceneName = "StartScene";
	
	const CollisionMngInterface* _CollisionMng;

	ScreenPoint _ScreenSize;

	Camera* _MainCamera = nullptr;
	Player* _MainPlayer = nullptr;
};