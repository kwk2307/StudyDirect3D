#pragma once
class EngineInterface {
public:

	virtual bool Init(const Viewport& InScreenSize) = 0;
	virtual bool IsInitialized() = 0;
	virtual void OnScreenResize(const Viewport& InScreenSize) = 0;
	virtual InputManager& GetInputManager() = 0;
};