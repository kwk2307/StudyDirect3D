#include "Precompiled.h"

void GameEngine::Update(float InDeltaSeconds)
{
	GetSceneMng().Update(InDeltaSeconds);
	GetCollisionMng().Update(InDeltaSeconds);
}

GameEngine::GameEngine(SceneMngInterface* InSceneMng, CollisionMngInterface* InCollisionMng) :
	_SceneMng(InSceneMng), _CollisionMng(InCollisionMng)
{
}

bool GameEngine::Init(const ScreenPoint& InScreenSize)
{
	_IsInitialized =  GetSceneMng().Init(_CollisionMng.get());
	assert(_IsInitialized);
	GetSceneMng().OnScreenResize(InScreenSize);

	GetCollisionMng().Init(_SceneMng.get());
	GetCollisionMng().CheckGroup(ObjectType::Player, ObjectType::Block);

	return _IsInitialized;
}

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	GetSceneMng().OnScreenResize(InScreenSize);
}
