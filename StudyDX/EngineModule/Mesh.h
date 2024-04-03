#pragma once

enum class MeshType
{
	Normal,

};

class Mesh {

public:
	Mesh() = default;
	Mesh(std::string InName,std::vector<Vector3> InVertices, std::vector<size_t> Indices, std::vector<Vector2> InUVs);

	~Mesh();
public:
	__forceinline bool HasUV() const { return _UVs.size() > 0; }

	std::vector<Vector3>& GetVertices() { return _Vertices; }
	const std::vector<Vector3>& GetVertices() const { return _Vertices; }
	std::vector<size_t>& GetIndices() { return _Indices; }
	const std::vector<size_t>& GetIndices() const { return _Indices; }
	std::vector<Vector2>& GetUVs() { return _UVs; }
	const std::vector<Vector2>& GetUVs() const { return _UVs; }

	void CalculateBounds();
	const Sphere& GetSphereBound() const { return _SphereBound; }
	const Box& GetBoxBound() const { return _BoxBound; }

private:
	std::string _Name;

	std::vector<Vector3> _Vertices;
	std::vector<size_t> _Indices;
	std::vector<Vector2> _UVs;

	MeshType _MeshType = MeshType::Normal;
	Sphere _SphereBound;
	Box _BoxBound;
};