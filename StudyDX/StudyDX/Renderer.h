#pragma once
#include <functional>
class Renderer
{
public:
	Renderer() = default;
	Renderer(RendererInterface* RI, EngineInterface* EI);
	
	void OnTick();
	void OnResize(const ScreenPoint& InNewScreenSize);
	void OnShutdown();

	float GetFrameFPS() const { return _FrameFPS; }
	FORCEINLINE float GetElapsedTime() const { return _ElapsedTime; }

	// 성능 측정
	std::function<float()> _PerformanceInitFunc;
	std::function<INT64()> _PerformanceMeasureFunc;
	std::function<void(InputManager&)> _InputBindingFunc;

	EngineInterface& GetGameEngine() { return *_ENIPtr.get(); }

private:
	// 기본 루프 함수
	void PreUpdate();
	void PostUpdate();

	void Update(float InDeltaSeconds);
	void LateUpdate(float InDeltaSeconds);
	void Render();

	void LoadScene();

	RendererInterface& GetRenderer() { return *_RSIPtr.get(); }

private:

	// 초기화 점검 변수
	bool _PerformanceCheckInitialized = false;
	bool _RendererInitialized = false;
	bool _GameEngineInitialized = false;
	bool _TickEnabled = false;
	bool _AllInitialized = false;

	// 성능 측정
	long long _StartTimeStamp = 0;
	long long _FrameTimeStamp = 0;
	long _FrameCount = 0;
	float _CyclesPerMilliSeconds = 0.f;
	float _FrameTime = 0.f;
	float _ElapsedTime = 0.f;
	float _AverageFPS = 0.f;
	float _FrameFPS = 0.f;

	Color _BackgroundColor = Color();

	ScreenPoint _ScreenSize;

	std::unique_ptr<RendererInterface> _RSIPtr;
	std::unique_ptr<EngineInterface> _ENIPtr;


};

