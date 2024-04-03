#pragma once

class RendererInterface {
public:
	virtual bool Init(const Viewport& InSize) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsInitialized() const = 0;

	virtual void Clear(const Color& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawPoint(const Viewport& InScreenPos, const Color& InColor) = 0;
	virtual void DrawLine(const Vector4& InStartPos, const Vector4& InEndPos, const Color& InColor) = 0;
	virtual void DrawFullVerticalLine(int InX, const Color& InColor) = 0;
	virtual void DrawFullHorizontalLine(int InY, const Color& InColor) = 0;
};