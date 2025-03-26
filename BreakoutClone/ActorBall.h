#pragma once

#include "Actor.h"

class ActorBall : public Actor
{
public:
	ActorBall(class Game* game);

	void RenderActorDebug(struct SDL_Renderer* renderer) override;

protected:
	void ResolveActorCollision() override;

private:
	void ResolveWallCollision(const struct AABB& box, Vector2D& vel, Vector2D& pos, float& rot);
	void ResolveBlockCollision(const struct AABB& box, Vector2D& vel, Vector2D& pos, float& rot);

	class SpriteComponent* mSpriteComp;
	class MoveComponent* mMoveComp;
	class CollisionComponent* mCollisionComp;
};

