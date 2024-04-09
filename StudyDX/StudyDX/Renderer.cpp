#include "Precompiled.h"
#include "Renderer.h"


Renderer::Renderer(RendererInterface* RI, EngineInterface* EI):
	_ENIPtr(EI),_RSIPtr(RI)
{
}


void Renderer::OnTick()
{
	if (!_AllInitialized)
	{
		// �����ս� ī���� �ʱ�ȭ.
		if (!_PerformanceCheckInitialized)
		{
			if (_PerformanceInitFunc && _PerformanceMeasureFunc)
			{
				_CyclesPerMilliSeconds = _PerformanceInitFunc();
				_PerformanceCheckInitialized = true;
			}
			else
			{
				assert(false);
				return;
			}
		}

		// ��ũ�� ũ�� Ȯ��
		if (_ScreenSize.HasZero())
		{
			return;
		}

		// ����Ʈ ������ �ʱ�ȭ.
		if (!_RendererInitialized)
		{
			_RendererInitialized = GetRenderer().Init(_ScreenSize);
			if (!_RendererInitialized)
			{
				assert(false);
				return;
			}
		}

		// ���� ���� �ʱ�ȭ
		_GameEngineInitialized = GetGameEngine().IsInitialized();
		if (!_GameEngineInitialized)
		{
			GetGameEngine().OnScreenResize(_ScreenSize);
			InputManager& input = GetGameEngine().GetInputManager();
			if (!input.IsInputReady())
			{
				_InputBindingFunc(input);
			}

			_GameEngineInitialized = GetGameEngine().Init();
			if (!_GameEngineInitialized)
			{
			
				assert(false);
				return;
			}
			
			LoadScene();

		}

		_AllInitialized = _RendererInitialized && _PerformanceCheckInitialized && _GameEngineInitialized;
		if (_AllInitialized)
		{
			GetRenderer()._GetObjectFunc = [this](const std::string& InName) -> GameObject& {
				return  GetGameEngine().GetGameObject(InName);
				};
			
			_TickEnabled = true;
		}
	}
	else
	{
		assert(_RSIPtr != nullptr && GetRenderer().IsInitialized() && !_ScreenSize.HasZero());

		if (_TickEnabled)
		{
			PreUpdate();

			// ���� ���� ��ü�� �Լ� ���� ����
			if (!_AllInitialized)
			{
				GetRenderer()._GetObjectFunc = nullptr;
				//GetSystemInput().UpdateSystemInput();
				return;
			}

			Update(_FrameTime / 1000.f);
			LateUpdate(_FrameTime / 1000.f);
			Render();

			PostUpdate();
		}
	}
}

void Renderer::OnResize(const ScreenPoint& InNewScreenSize)
{
	_ScreenSize = InNewScreenSize;

	if (_RendererInitialized) {
		GetRenderer().OnReSize(InNewScreenSize);
	}
	if (_GameEngineInitialized) {
		GetGameEngine().OnScreenResize(InNewScreenSize);
	}
}

void Renderer::OnShutdown()
{
	GetRenderer().Shutdown();
}

void Renderer::PreUpdate()
{
	// ���� ���� ����.
	_FrameTimeStamp = _PerformanceMeasureFunc();
	if (_FrameCount == 0)
	{
		_StartTimeStamp = _FrameTimeStamp;
	}
	// ��� �����.
	GetRenderer().Clear(_BackgroundColor);

}

void Renderer::PostUpdate()
{
	// ������ ������.
	GetRenderer().EndFrame();

	// ���� ���� ������.
	_FrameCount++;
	INT64 currentTimeStamp = _PerformanceMeasureFunc();
	INT64 frameCycles = currentTimeStamp - _FrameTimeStamp;
	INT64 elapsedCycles = currentTimeStamp - _StartTimeStamp;
	_FrameTime = frameCycles / _CyclesPerMilliSeconds;
	_ElapsedTime = elapsedCycles / _CyclesPerMilliSeconds;
	_FrameFPS = _FrameTime == 0.f ? 0.f : 1000.f / _FrameTime;
	_AverageFPS = _ElapsedTime == 0.f ? 0.f : 1000.f / _ElapsedTime * _FrameCount;
}

void Renderer::Update(float InDeltaSeconds)
{ 
	auto& g = GetGameEngine();

	g.GetCamera().GetTransform().SetLocalPosition(Vector3(0, 0, -2));

	for (auto iter = g.SceneBegin(); iter != g.SceneEnd(); ++iter) {
		GameObject obj = *(*iter);
		obj.Update(InDeltaSeconds);
	}
}

void Renderer::LateUpdate(float InDeltaSeconds)
{
}

void Renderer::Render()
{
	auto& g = GetGameEngine();
	auto& r = GetRenderer();
	
	r.Render();

	for (auto iter = g.SceneBegin(); iter != g.SceneEnd(); ++iter) {
		GameObject obj = *(*iter);
		if (!obj.HasMesh() || !obj.IsVisible()) {
			continue;
		}
		obj.Render();
	}
}

void Renderer::LoadScene()
{
	auto& g = GetGameEngine();
	
	CreateObject("Player", GameEngine::BoxMesh, Transform(Vector3(0, 0, 0)));

}

void Renderer::CreateObject(const std::string& InName, const std::size_t& InMeshKey, const Transform& InTransform)
{
	auto& g = GetGameEngine();
	auto& r = GetRenderer();

	GameObject& go = g.CreateNewGameObject(InName);
	
	go.GetTransform().SetWorldTransform(InTransform);

	if (InMeshKey != NULL) {

		const MeshData& meshData = g.GetMesh(InMeshKey);

		go.SetMesh(r.CreateMesh(meshData));
		
		go._OnUpdateFunc = [this,&go]() {
			GetRenderer().OnUpdateEvnet(
				go.GetMesh(),
				go.GetTransform().GetLocalMatrix(), 
				GetGameEngine().GetCamera().GetViewMatrix(), 
				GetGameEngine().GetCamera().GetPerspectiveMatrix());
			};
			
		go._OnRenderFunc = [this,&go]() {
			GetRenderer().OnRenderEvent(go.GetMesh());
			};
	}

}
