#pragma once

struct Mesh;

class RendererInterface {
public:
	virtual bool Init(const ScreenPoint& InSize) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsInitialized() const = 0;

	virtual bool OnReSize(const ScreenPoint& InSize) = 0;

	virtual void Clear(const Color& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual std::shared_ptr<Mesh> CreateMesh(const MeshData& InMeshData) = 0;

	virtual void OnUpdateEvnet(std::shared_ptr<Mesh> InMesh, const Matrix& InTransform, const Matrix& InView, const Matrix& InProj) = 0;
	virtual void OnRenderEvent(std::shared_ptr<Mesh>InMesh) = 0;

	std::function<GameObject& (const std::string& InName)> _GetObjectFunc;
	
	virtual void Render() = 0;
};