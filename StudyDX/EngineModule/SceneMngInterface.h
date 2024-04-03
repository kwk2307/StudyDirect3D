#pragma once

class CollisionMngInterface;

class SceneMngInterface
{
public:
	virtual bool Init(const CollisionMngInterface* InCollisionMng) = 0;
	virtual const std::vector<std::shared_ptr<Object>>& GetCurrentScene(UINT InType) const = 0;

	virtual void Update(float InDeltaSeconds) = 0;

	virtual Camera& GetCamera() = 0;
	virtual Player& GetPlayer() = 0;

	virtual Mesh& GetMesh(const std::size_t& InMeshKey) const = 0;
	virtual Texture& GetTexture(const std::size_t InTextureKey) const = 0;

	virtual void OnScreenResize(const ScreenPoint& InScreenSize) = 0;

private:
};
