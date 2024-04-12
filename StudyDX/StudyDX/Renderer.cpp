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
		// 퍼포먼스 카운터 초기화.
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

		// 스크린 크기 확인
		if (_ScreenSize.HasZero())
		{
			return;
		}

		// 소프트 렌더러 초기화.
		if (!_RendererInitialized)
		{
			_RendererInitialized = GetRenderer().Init(_ScreenSize);
			if (!_RendererInitialized)
			{
				assert(false);
				return;
			}
		}

		// 게임 엔진 초기화
		_GameEngineInitialized = GetGameEngine().IsInitialized();
		if (!_GameEngineInitialized)
		{
			GetGameEngine().OnScreenResize(_ScreenSize);
			InputManager& input = GetGameEngine().GetInputManager();
			if (!input.IsInputReady())
			{
				_InputBindingFunc(input);
			}

			GetGameEngine()._OnCreateMesh =
				[this](const std::size_t& InMeshKey, std::vector<Vertex> InVertices, std::vector<uint32_t> InIndices, std::string InTextureFilename) {
				GetRenderer().CreateMesh(InMeshKey, InVertices, InIndices, InTextureFilename);
				};
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
			_TickEnabled = true;
		}
	}
	else
	{
		assert(_RSIPtr != nullptr && GetRenderer().IsInitialized() && !_ScreenSize.HasZero());

		if (_TickEnabled)
		{
			ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame(); // 어떤 것들을 렌더링 할지 기록 시작
			ImGui::Begin("Scene Control");

			// ImGui가 측정해주는 Framerate 출력
			ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate);

			UpdateGUI(); // 추가적으로 사용할 GUI
		
			ImGui::End();
			ImGui::Render(); // 렌더링할 것들 기록 끝

			PreUpdate();

			// 게임 엔진 교체로 함수 리셋 진행
			if (!_AllInitialized)
			{
				GetGameEngine()._OnCreateMesh = nullptr;
				//GetSystemInput().UpdateSystemInput();
				return;
			}

			Update(_FrameTime / 1000.f);
			LateUpdate(_FrameTime / 1000.f);
			Render();

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링

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
	// 성능 측정 시작.
	_FrameTimeStamp = _PerformanceMeasureFunc();
	if (_FrameCount == 0)
	{
		_StartTimeStamp = _FrameTimeStamp;
	}
	GetRenderer().BeginFrame();
	// 배경 지우기.
	GetRenderer().Clear(_BackgroundColor);
}

void Renderer::PostUpdate()
{
	// 렌더링 마무리.
	GetRenderer().EndFrame();

	// 성능 측정 마무리.
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
	
	for (auto iter = g.SceneBegin(); iter != g.SceneEnd(); ++iter) {
		GameObject obj = *(*iter);
		if (!obj.HasMesh() || !obj.IsVisible()) {
			continue;
		}
		obj.Render();
	}
}

void Renderer::UpdateGUI()
{

	//ImGui::Checkbox("Use Texture", &m_BasicPixelConstantBufferData.useTexture);
	//ImGui::Checkbox("Wireframe", &m_drawAsWire);
	//ImGui::Checkbox("Draw Normals", &m_drawNormals);
	//if (ImGui::SliderFloat("Normal scale",
	//	&m_normalVertexConstantBufferData.scale, 0.0f,
	//	1.0f)) {
	//	m_drawNormalsDirtyFlag = true;
	//}
	//ImGui::SliderFloat3("m_modelTranslation", &m_modelTranslation.x, -2.0f,
	//	2.0f);
	//ImGui::SliderFloat3("m_modelRotation", &m_modelRotation.x, -3.14f, 3.14f);
	//ImGui::SliderFloat3("m_modelScaling", &m_modelScaling.x, 0.1f, 2.0f);
	//ImGui::SliderFloat3("m_viewRot", &m_viewRot.x, -3.14f, 3.14f);

	//ImGui::SliderFloat("Material Shininess",
	//	&m_BasicPixelConstantBufferData.material.shininess, 1.0f,
	//	256.0f);

	//if (ImGui::RadioButton("Directional Light", m_lightType == 0)) {
	//	m_lightType = 0;
	//}
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Point Light", m_lightType == 1)) {
	//	m_lightType = 1;
	//}
	//ImGui::SameLine();
	//if (ImGui::RadioButton("Spot Light", m_lightType == 2)) {
	//	m_lightType = 2;
	//}

	//ImGui::SliderFloat("Material Diffuse", &m_materialDiffuse, 0.0f, 1.0f);
	//ImGui::SliderFloat("Material Specular", &m_materialSpecular, 0.0f, 1.0f);

	//ImGui::SliderFloat3("Light Position", &m_lightFromGUI.position.x, -5.0f,
	//	5.0f);

	//ImGui::SliderFloat("Light fallOffStart", &m_lightFromGUI.fallOffStart, 0.0f,
	//	5.0f);

	//ImGui::SliderFloat("Light fallOffEnd", &m_lightFromGUI.fallOffEnd, 0.0f,
	//	10.0f);

	//ImGui::SliderFloat("Light spotPower", &m_lightFromGUI.spotPower, 1.0f,
	//	512.0f);
}

void Renderer::LoadScene()
{
	auto& g = GetGameEngine();
	CreateObject("Box", std::hash<std::string>() ("zelda"), Transform(Vector3(-0.5f, 0.f, 0.f), Quaternion(), Vector3(1.f)));

	CreateObject("Box2", std::hash<std::string>() ("zelda"), Transform(Vector3(0.5f, 0.f, 0.f), Quaternion(), Vector3(1.f)));
}

void Renderer::CreateObject(const std::string& InName, const std::size_t& InMeshKey, const Transform& InTransform)
{
	auto& g = GetGameEngine();
	auto& r = GetRenderer();

	GameObject& go = g.CreateNewGameObject(InName);
	go.GetTransform().SetLocalTransform(InTransform);

	if (InMeshKey != NULL) {
		go.SetMesh(InMeshKey);
		
		go._OnUpdateFunc = [this,&go]() {

			};
			
		go._OnRenderFunc = [this,&go]() {
			GetRenderer().OnRenderEvent(
				go.GetMeshKey(),
				go.GetTransform().GetLocalMatrix(),
				GetGameEngine().GetCamera().GetViewMatrix(),
				GetGameEngine().GetCamera().GetPerspectiveMatrix()
			);
			};
	}
}
