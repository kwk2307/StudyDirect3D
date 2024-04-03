#pragma once
struct Vertex {

public:
	constexpr Vertex() = default;
	constexpr Vertex(const Vector4& InPosition) : Position(InPosition) { }
	constexpr Vertex(const Vector4& InPosition, const Color& InColor) : Position(InPosition), Color(InColor) { }
	constexpr Vertex(const Vector4& InPosition, const Color& InColor, const Vector2& InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

	Vector4 Position;
	Color Color;
	Vector2 UV;
};