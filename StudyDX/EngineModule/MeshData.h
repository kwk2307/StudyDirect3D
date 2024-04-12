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
	MeshData(const std::vector<Vertex>& InVertices, const std::vector<uint32_t> InIndicies, std::string InFileName):
		_Vertices(InVertices),_Indices(InIndicies),_TextureFilename(InFileName)
	{}

public:
	void SetMeshType(const MeshType& _InMeshType) { _MeshType = _InMeshType; }
	std::vector<Vertex>& GetVertices() { return _Vertices; }
	const std::vector<Vertex>& GetVertices() const { return _Vertices; }
	std::vector<uint32_t>& GetIndices() { return _Indices; }
	const std::vector<uint32_t>& GetIndices() const { return _Indices; }

	std::string& GetTexture() { return _TextureFilename; }
	const std::string& GetTexture() const { return _TextureFilename; }

	std::vector<Vertex> _Vertices;
	std::vector<uint32_t> _Indices;

	std::string _TextureFilename;

private:
	MeshType _MeshType = MeshType::Normal;
	DirectX::BoundingBox Box;
};

