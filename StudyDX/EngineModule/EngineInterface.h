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

	virtual const std::vector<std::shared_ptr<MeshData>>& GetMesh(const std::size_t& InMeshKey) const = 0;

	std::function<void(const std::size_t& InMeshKey, std::vector<Vertex> InVertices, std::vector<uint32_t> InIndices, std::string InTextureFilename)> _OnCreateMesh;
};