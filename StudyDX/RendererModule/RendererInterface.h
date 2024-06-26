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

	virtual void CreateMesh(const std::size_t& InMeshkey, const std::vector<Vertex>& InVertices, const std::vector<std::uint32_t> InIndices, const std::string InFileName) = 0;

	//virtual void OnUpdateEvnet(std::shared_ptr<Mesh> InMesh, const Matrix& InTransform, const Matrix& InView, const Matrix& InProj) = 0;
	virtual void OnRenderEvent(const std::size_t& InMeshKey, const Matrix& InTransform, const Matrix& InView, const Matrix& InProj) = 0;

};