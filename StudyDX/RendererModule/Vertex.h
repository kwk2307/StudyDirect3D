#pragma once
struct Vertex {

public:
	Vertex() = default;
	Vertex(const Vector4& InPosition) : Position(InPosition) { }
	Vertex(const Vector4& InPosition, const Color& InColor) : Position(InPosition), Color(InColor) { }
	Vertex(const Vector4& InPosition, const Color& InColor, const Vector2& InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

	Vector4 Position;
	Color Color;
	Vector2 UV;
};