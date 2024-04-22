#pragma once

class GameEngine : public EngineInterface
{
public:
	GameEngine() = default;

public:
	virtual bool Init() override;
	virtual bool IsInitialized() override { return _IsInitialized; }
	virtual void OnScreenResize(const ScreenPoint& InScreenSize) override;
	virtual InputManager& GetInputManager() override { return _InputManager; }

	//게임 오브젝트 
	const std::vector<std::unique_ptr<GameObject>>& GetScene() const { return _Scene; }
	virtual std::vector< std::unique_ptr<GameObject>>::const_iterator SceneBegin() const override{ return _Scene.begin(); }
	virtual std::vector< std::unique_ptr<GameObject>>::const_iterator SceneEnd() const override { return _Scene.end(); }
	virtual GameObject& GetGameObject(const std::string& InName) override;
	virtual GameObject& CreateNewGameObject(const std::string& InName) override;

	virtual CameraObject& GetCamera() { return _MainCamera; }

	//리소스 관리 
	
	std::vector<std::shared_ptr<MeshData>>& CreateMesh(const std::size_t& InKey);
	std::vector<std::shared_ptr<MeshData>>& CreateMesh(const std::size_t& InKey, const MeshData& InMesh);
	std::vector<std::shared_ptr<MeshData>>& CreateMesh(const std::size_t& InKey, std::string InBasePath, std::string Infilename);

	std::vector<MeshData> ReadFromFile(std::string InBasePath, std::string Infilename);

	std::shared_ptr<TextureData>& CreateTexture(const std::size_t& InKey, std::string InBasePath, std::string Infilename);

	// 메시
	virtual const std::vector<std::shared_ptr<MeshData>>& GetMesh(const std::size_t& InMeshKey) const {
		return _Meshes.at(InMeshKey);
	}
	
	// 카메라 
	FORCEINLINE CameraObject& GetMainCamera() { return _MainCamera; }
	FORCEINLINE const CameraObject& GetMainCamera() const { return _MainCamera; }

private:
	bool LoadResources();

public:
	static const std::size_t RectangleMesh;
	static const std::size_t BoxMesh;
	static const std::size_t SphereMesh;

private:
	bool _IsInitialized = false;
	ScreenPoint _ScreenSize;

	InputManager _InputManager;

	std::vector<std::unique_ptr<GameObject>> _Scene;
	
	std::unordered_map<std::size_t, std::vector<std::shared_ptr<MeshData>>> _Meshes;
	std::unordered_map<std::size_t, std::shared_ptr<TextureData>> _Textures;

	CameraObject _MainCamera;
};



