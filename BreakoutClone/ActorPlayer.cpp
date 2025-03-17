#include "ActorPlayer.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL.h"

#include <algorithm>

ActorPlayer::ActorPlayer(Game* game)
	: Actor(game), mMousePosX(0.0f), mIsMouseEnabled(false)
{
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->SetTexture(GetGame()->GetTexture("Assets/Paddle_Red.png"));

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetMaxForwardSpeed(300.0f);

	mCollisionComp = new CollisionComponent(this);
	float halfTexWidth = mSpriteComp->GetTextureWidth() * 0.5f;
	float halfTexHeight = mSpriteComp->GetTextureHeight() * 0.5f;
	Vector2D min{ -halfTexWidth, -halfTexHeight };
	Vector2D max{ halfTexWidth, halfTexHeight };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);

	SetPosition(Vector2D(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 50.0f));
}

void ActorPlayer::ProcessActorInput(const InputState& inputState)
{
	float maxForwardSpeed = mMoveComp->GetMaxForwardSpeed();
	float forwardSpeed = 0.0f;
	
	// Handle keyboard input
	if (inputState.keyboardState[SDL_SCANCODE_D]) // Move Right
	{
		forwardSpeed += maxForwardSpeed;
	}
	if (inputState.keyboardState[SDL_SCANCODE_A]) // Move Left
	{
		forwardSpeed += -maxForwardSpeed;
	}

	// Handle mouse input
	mIsMouseEnabled = false;
	if (inputState.isMouseEnabled)
	{
		mIsMouseEnabled = true;
		mMousePosX = static_cast<int>(inputState.mouseX);

		// If actor is not at mouse position, move toward it
		float currPosX = GetPosition().x;
		if (currPosX != mMousePosX)
		{
			forwardSpeed += (currPosX < mMousePosX) ? maxForwardSpeed : -maxForwardSpeed;
		}
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
}

void ActorPlayer::UpdateActor(float deltaTime)
{
	// Actor-Specific Collision
	// Clamp Actor(paddle) position to mouse (if mouse enabled) and screen edges
	if (!Math::IsNearZero(mMoveComp->GetForwardSpeed()))
	{
		Vector2D pos = GetPosition();

		if (mIsMouseEnabled)
		{
			pos.x = ClampPositionToMouse(pos.x);
		}

		pos.x = ResolveWallCollision(pos.x);

		SetPosition(pos);
	}
}

// Limit horizontal movement to the edge of screen
float ActorPlayer::ResolveWallCollision(float inPosX)
{
	float posX = inPosX;

	float halfSpriteWidth = (mSpriteComp->GetScaledWidth() * 0.5f);
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

// If mouse is enabled, prevent paddle from overshooting mouse position x
float ActorPlayer::ClampPositionToMouse(float inPosX)
{
	float posX = inPosX;
	
	float forwardSpeed = mMoveComp->GetForwardSpeed();
	if (forwardSpeed > 0.0f)
	{
		posX = std::min(posX, mMousePosX);
	}
	else if (forwardSpeed < 0.0f)
	{
		posX = std::max(posX, mMousePosX);
	}

	return posX;
}

void ActorPlayer::RenderActorDebug(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	const AABB& box = mCollisionComp->GetWorldAABB();
	SDL_Rect debugBox = {
		static_cast<int>(box.min.x),
		static_cast<int>(box.min.y),
		static_cast<int>(box.max.x - box.min.x),
		static_cast<int>(box.max.y - box.min.y)
	};
	SDL_RenderDrawRect(renderer, &debugBox);
}