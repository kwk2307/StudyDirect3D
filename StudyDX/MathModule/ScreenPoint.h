#pragma once

struct ScreenPoint
{
public:
	constexpr ScreenPoint() = default;
	explicit constexpr ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	explicit constexpr ScreenPoint(float InX, float InY) : X(MathUtil::FloorToInt(InX)), Y(MathUtil::FloorToInt(InY)) { }
	explicit constexpr ScreenPoint(const Vector2& InPos) : ScreenPoint(InPos.x, InPos.y) {}

	constexpr ScreenPoint GetHalf() const { return ScreenPoint(MathUtil::FloorToInt(0.5f * X), MathUtil::FloorToInt(0.5f * Y)); }
	constexpr float AspectRatio() const { return (float)X / (float)Y; } // Y축 기준으로 측정
	constexpr bool HasZero() const { return (X == 0 || Y == 0); }

	static constexpr ScreenPoint ToScreenCoordinate(const ScreenPoint& InScreenSize, const Vector2& InPos)
	{
		return ScreenPoint(InPos.x + InScreenSize.X * 0.5f, -InPos.y + InScreenSize.Y * 0.5f);
	}

	constexpr Vector2 ToCartesianCoordinate(const ScreenPoint& InScreenSize)
	{
		return Vector2(X - InScreenSize.X * 0.5f + 0.5f, -(Y + 0.5f) + InScreenSize.Y * 0.5f);
	}

	constexpr ScreenPoint operator-(const ScreenPoint& InPoint) const;
	constexpr ScreenPoint operator+(const ScreenPoint& InPoint) const;

	int X = 0;
	int Y = 0;
};

constexpr ScreenPoint ScreenPoint::operator-(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X - InPoint.X, Y - InPoint.Y);
}

constexpr ScreenPoint ScreenPoint::operator+(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X + InPoint.X, Y + InPoint.Y);
}