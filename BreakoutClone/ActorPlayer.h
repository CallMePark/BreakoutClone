#pragma once

#include "Actor.h"

class ActorPlayer : public Actor
{
public:
	ActorPlayer(class Game* game);

	void ProcessActorInput(const struct InputState& inputState) override;
	void UpdateActor(float deltaTime) override;
	void RenderActorDebug(struct SDL_Renderer* renderer) override;

private:
	class SpriteComponent* mSpriteComp;
	class MoveComponent* mMoveComp;
	class CollisionComponent* mCollisionComp;

	float mMousePosX;
	bool mIsMouseEnabled;

	float ResolveWallCollision(float inPosX);
	float ClampPositionToMouse(float inPosX);
};