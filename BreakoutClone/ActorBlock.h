#pragma once

#include "Actor.h"

class ActorBlock : public Actor
{
public:
	ActorBlock(class Game* game);

	void RenderActorDebug(struct SDL_Renderer* renderer) override;

private:
	class SpriteComponent* mSpriteComp;
	class CollisionComponent* mCollisionComp;
};

