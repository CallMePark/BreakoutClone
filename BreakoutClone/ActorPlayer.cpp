#include "ActorPlayer.h"
#include "Game.h"
#include "SpriteComponent.h"

#include <cmath>
#include <algorithm>

ActorPlayer::ActorPlayer(Game* game)
	: Actor(game), mMaxSpeed(400.0f), mMoveDir(0.0f), mMousePosX(0.0f), mIsMouseEnabled(false)
{
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->SetTexture(GetGame()->GetTexture("Assets/Paddle_Red.png"));

	SetPosition(Vector2D(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50.0f));
}

void ActorPlayer::ActorInput(const InputState& inputState)
{
	// Handle keyboard input
	mMoveDir = 0.0f;
	if (inputState.keyboardState[SDL_SCANCODE_D])
	{
		mMoveDir += 1.0f;
	}
	if (inputState.keyboardState[SDL_SCANCODE_A])
	{
		mMoveDir -= 1.0f;
	}

	// Handle mouse input
	mIsMouseEnabled = false;
	if (inputState.isMouseEnabled)
	{
		mIsMouseEnabled = true;
		mMousePosX = (float)inputState.mouseX;

		float currPosX = GetPosition().x;
		if (currPosX != mMousePosX)
		{
			mMoveDir = (currPosX < mMousePosX) ? 1.0f : -1.0f;
		}
	}
}

void ActorPlayer::UpdateActor(float deltaTime)
{
	// Handle keyboard/mouse movement
	if (!IsNearZero(mMoveDir))
	{
		Vector2D pos = GetPosition();
		pos.x += mMoveDir * mMaxSpeed * deltaTime;

		// If mouse is enabled, prevent paddle from overshooting mouse position x
		if (mIsMouseEnabled)
		{
			pos.x = std::clamp(pos.x, 
				std::min(GetPosition().x, mMousePosX),	// Min before update
				std::max(GetPosition().x, mMousePosX)	// Max before update
			);
		}

		pos.x = CalcEdgeCollision(pos.x);
		SetPosition(pos);
	}
}

// Limit horizontal movement to the edge of screen
float ActorPlayer::CalcEdgeCollision(float inPosX)
{
	float posX = inPosX;

	float halfSpriteWidth = (mSpriteComp->GetTextureWidth() / 2.0f);
	if (posX - halfSpriteWidth < 0.0f)
	{
		posX = 0.0f + halfSpriteWidth;
	}
	else if (posX + halfSpriteWidth > WINDOW_WIDTH)
	{
		posX = WINDOW_WIDTH - halfSpriteWidth;
	}

	return posX;
}

bool ActorPlayer::IsNearZero(float value, float epsilon)
{
	if (fabs(value) <= epsilon)
	{
		return true;
	}
	else
	{
		return false;
	}
}
