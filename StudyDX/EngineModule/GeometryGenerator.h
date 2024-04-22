#pragma once
class GeometryGenerator
{
public:

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
};

