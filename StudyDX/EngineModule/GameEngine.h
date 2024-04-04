#pragma once

class GameEngine : public EngineInterface
{
public:
	GameEngine(SceneMngInterface* InSceneMng, CollisionMngInterface* InCollisionMng);

public:
	virtual bool Init(const ScreenPoint& InScreenSize) override;
	virtual bool IsInitialized() override { return _IsInitialized; }

	virtual void OnScreenResize(const ScreenPoint& InScreenSize) override;

	SceneMngInterface& GetSceneMng() { return *_SceneMng.get(); }
	CollisionMngInterface& GetCollisionMng() { return *_CollisionMng.get(); }

	//virtual InputManager& GetInputManager() override { return _InputManager; }

	void Update(float InDeltaSeconds);

private:
	bool _IsInitialized = false;
	
	std::unique_ptr<SceneMngInterface> _SceneMng;
	std::unique_ptr<CollisionMngInterface> _CollisionMng;

	InputManager _InputManager;
};



