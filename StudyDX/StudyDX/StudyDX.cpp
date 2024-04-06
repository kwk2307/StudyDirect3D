// StudyDX.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

// 지역변수는 소문자로 시작
// 정적변수는 g붙이고 시작
// 멤버변수는 _붙이고 시작
// 함수는 대문자로 시작
// 매게변수는 In붙이고 시작


#include "Precompiled.h"
#include "Renderer.h"
#include "WindowsUtil.h"
#include "WindowsPlayer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	ScreenPoint defScreenSize(800, 600);
	Renderer Instance(new DXRenderer(), new GameEngine());

	WindowsPlayer::gOnResizeFunc = [&Instance](const ScreenPoint& InNewScreenSize) {
		if (InNewScreenSize.HasZero()) {
			return;
		}
		Instance.OnResize(InNewScreenSize);
		};

	Instance._PerformanceInitFunc = WindowsUtil::GetCyclesPerMilliSeconds;
	Instance._PerformanceMeasureFunc = WindowsUtil::GetCurrentTimeStamp;
	Instance._InputBindingFunc = WindowsUtil::BindInput;
	//WindowsUtil::BindSystemInput(instance.GetSystemInput());

	if (!WindowsPlayer::Create(hInstance, defScreenSize))
	{
		return -1;
	}

	WindowsUtil::Show(WindowsPlayer::gHandle);
	WindowsUtil::CenterWindow(WindowsPlayer::gHandle);

	static float previousTimer = 0.f;
	static float updatePeriod = 500.f;
	while (WindowsPlayer::Tick())
	{
		Instance.OnTick();

		float currentTime = Instance.GetElapsedTime();
		if (currentTime - previousTimer > updatePeriod)
		{
			float frameFPS = Instance.GetFrameFPS();
			WindowsPlayer::SetWindowsStatTitle(frameFPS);
			previousTimer = currentTime;
		}
	}

	Instance.OnShutdown();
	WindowsPlayer::Destroy();
	return 0;
}

