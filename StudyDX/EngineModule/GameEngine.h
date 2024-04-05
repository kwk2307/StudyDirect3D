#pragma once

class GameEngine : public EngineInterface
{
public:
	GameEngine();

public:
	virtual bool Init() override;
	virtual bool IsInitialized() override { return _IsInitialized; }
	virtual void OnScreenResize(const ScreenPoint& InScreenSize) override;

	virtual InputManager& GetInputManager() override { return _InputManager; }

	void Update(float InDeltaSeconds);

private:
	bool _IsInitialized = false;
	ScreenPoint _ScreenSize;

	InputManager _InputManager;

	std::vector<std::unique_ptr<GameObject>> _Scene;
	std::unordered_map<std::size_t, std::unique_ptr<Mesh>> _Meshes;
	std::unordered_map<std::size_t, std::unique_ptr<Texture>> _Textures;
};



