#pragma once
class EngineInterface {
public:

	virtual bool Init(const ScreenPoint& InScreenSize) = 0;
	virtual bool IsInitialized() = 0;
	virtual void OnScreenResize(const ScreenPoint& InScreenSize) = 0;
	//virtual InputManager& GetInputManager() = 0;
};