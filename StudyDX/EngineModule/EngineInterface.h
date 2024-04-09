#pragma once

class EngineInterface {
public:

	virtual bool Init() = 0;
	virtual bool IsInitialized() = 0;
	virtual void OnScreenResize(const ScreenPoint& InScreenSize) = 0;
	virtual InputManager& GetInputManager() = 0;
	
	virtual std::vector< std::unique_ptr<GameObject>>::const_iterator SceneBegin() const = 0;
	virtual std::vector< std::unique_ptr<GameObject>>::const_iterator SceneEnd() const = 0;

	virtual GameObject& GetGameObject(const std::string& InName) = 0;
	virtual GameObject& CreateNewGameObject(const std::string& InName) = 0;

	virtual CameraObject& GetCamera() = 0;

	virtual MeshData& GetMesh(const std::size_t& InMeshKey) = 0;
	
};