#pragma once
class WindowsRenderer: public RendererInterface
{
public:
	virtual bool Init(const ScreenPoint& InSize) override;
	virtual void Shutdown() override;
	virtual bool IsInitialized() const { return _Initialized; }

	virtual void Clear(const Color& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

public:
	virtual void DrawPoint(const ScreenPoint& InScreenPos, const Color& InColor) override;
	virtual void DrawLine(const Vector4& InStartPos, const Vector4& InEndPos, const Color& InColor) override;

	virtual void DrawFullVerticalLine(int InX, const Color& InColor);
	virtual void DrawFullHorizontalLine(int InY, const Color& InColor);
private:
	void Release();
	void FillBuffer(Color InColor);

	bool IsInScreen(const ScreenPoint& InPos) const;
	int  GetScreenBufferIndex(const ScreenPoint& InPos) const;
	bool CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos);
	int TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos);

	Color GetPixel(const ScreenPoint& InPos);

	template <class T>
	T* CopyBuffer(T* InDst, T* InSrc, int InCount);

	void SwapBuffer();

private:
	bool _Initialized = false;

	HWND _Handle = 0;
	HDC	_ScreenDC = 0, _MemoryDC = 0;
	HBITMAP _DefaultBitmap = 0, DIBitmap = 0;

	Color* _ScreenBuffer = nullptr;

	ScreenPoint _ScreenSize;

};

