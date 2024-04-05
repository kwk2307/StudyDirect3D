#pragma once
class Mesh
{
public:
	Mesh() = default;
public:
	void SetMeshType(const MeshType& _InMeshType) { _MeshType = _InMeshType; }
	std::vector<Vector3>& GetVertices() { return _Vertices; }
	const std::vector<Vector3>& GetVertices() const { return _Vertices; }
	std::vector<size_t>& GetIndices() { return _Indices; }
	const std::vector<size_t>& GetIndices() const { return _Indices; }

private:
	std::vector<Vector3> _Vertices;
	std::vector<size_t> _Indices;

private:
	MeshType _MeshType = MeshType::Normal;

	DirectX::BoundingBox Box;
};

