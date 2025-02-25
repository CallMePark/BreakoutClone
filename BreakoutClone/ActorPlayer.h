#pragma once

#include "Actor.h"

class ActorPlayer : public Actor
{
public:
	ActorPlayer(class Game* game);

	void ActorInput(const struct InputState& inputState) override;
	void UpdateActor(float deltaTime) override;

private:
	class SpriteComponent* mSpriteComp;

	float mMaxSpeed;
	float mMoveDir;

	float mMousePosX;
	bool mIsMouseEnabled;

	float CalcEdgeCollision(float inPosX);
	bool IsNearZero(float value, float epsilon = 0.0001f); // HELPER FUNCTION
};