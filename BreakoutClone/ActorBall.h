#pragma once

#include "Actor.h"

class ActorBall : public Actor
{
public:
	ActorBall(class Game* game);

	void UpdateActor(float deltaTime) override;
	void RenderActorDebug(struct SDL_Renderer* renderer) override;

private:
	class SpriteComponent* mSpriteComp;
	class MoveComponent* mMoveComp;
	class CollisionComponent* mCollisionComp;

	float ClampPositionToScreenEdge(float inPosX);
};

