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
	MeshData& CreateMesh(const std::size_t& InKey);

	// 메시
	virtual MeshData& GetMesh(const std::size_t& InMeshKey) override { return *_Meshes.at(InMeshKey).get(); }

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

	std::unordered_map<std::size_t, std::unique_ptr<MeshData>> _Meshes;

	CameraObject _MainCamera;
};



