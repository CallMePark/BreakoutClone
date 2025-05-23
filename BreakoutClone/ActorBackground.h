#pragma once

#include "Actor.h"

#include <unordered_map>

class ActorBackground : public Actor
{
public:
	ActorBackground(class Game* game);

	void RenderActorDebug(struct SDL_Renderer* renderer) override;

private:
	class CollisionComponent* mCollisionComp;
};

