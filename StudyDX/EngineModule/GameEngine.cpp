#include "Precompiled.h"

struct GameObjectCompare
{
	bool operator()(const std::unique_ptr<GameObject>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

void GameEngine::Update(float InDeltaSeconds)
{
}

GameEngine::GameEngine()
{
}

bool GameEngine::Init()
{
	return _IsInitialized;
}

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{

}
