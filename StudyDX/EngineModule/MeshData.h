#pragma once

enum class MeshType : UINT32
{
	Normal = 0,
	Skinned
};

class MeshData
{
public:
	MeshData() = default;
public:
	void SetMeshType(const MeshType& _InMeshType) { _MeshType = _InMeshType; }
	std::vector<Vertex>& GetVertices() { return _Vertices; }
	const std::vector<Vertex>& GetVertices() const { return _Vertices; }
	std::vector<uint32_t>& GetIndices() { return _Indices; }
	const std::vector<uint32_t>& GetIndices() const { return _Indices; }

	std::string& GetTexture() { return _TextureFilename; }
	const std::string& GetTexture() const { return _TextureFilename; }

	static std::vector<MeshData> ReadFromFile(std::string INbasePath, std::string Infilename);

	static MeshData MakeSquare();
	static MeshData MakeBox();
	static MeshData MakeGrid(const float width, const float height,
		const int numSlices, const int numStacks);
	static MeshData MakeCylinder(const float bottomRadius,
		const float topRadius, float height,
		int sliceCount);
	static MeshData MakeSphere(const float radius, const int numSlices,
		const int numStacks);
	static MeshData SubdivideToSphere(const float radius, MeshData meshData);

private:
	std::vector<Vertex> _Vertices;
	std::vector<uint32_t> _Indices;

	std::string _TextureFilename;
private:
	MeshType _MeshType = MeshType::Normal;

	DirectX::BoundingBox Box;
};

