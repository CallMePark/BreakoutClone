#include "ActorBall.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

ActorBall::ActorBall(Game* game)
	: Actor(game)
{
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->SetTexture(GetGame()->GetTexture("Assets/Ball_Grey.png"));

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetMaxForwardSpeed(300.0f);
	mMoveComp->SetForwardSpeed(mMoveComp->GetMaxForwardSpeed());

	mCollisionComp = new CollisionComponent(this);
	float halfTexWidth = mSpriteComp->GetTextureWidth() * 0.5f;
	float halfTexHeight = mSpriteComp->GetTextureHeight() * 0.5f;
	Vector2D min{ -halfTexWidth, -halfTexHeight };
	Vector2D max{ halfTexWidth, halfTexHeight };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);

	SetPosition(Vector2D(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 100.0f));
	
	float startDirDegree = 135.0f;
	float startDirRadian = Math::ToRadian(startDirDegree);
	SetRotation(startDirRadian);
}

void ActorBall::UpdateActor(float deltaTime)
{
	ResolveWallCollision();
}

void ActorBall::ResolveWallCollision()
{
	float halfSpriteWidth = (mSpriteComp->GetScaledWidth() * 0.5f);
	float halfSpriteHeight = (mSpriteComp->GetScaledHeight() * 0.5f);

	const AABB& box = mCollisionComp->GetWorldAABB();

	// Set position to collision point and rotate accordingly
	Vector2D vel = mMoveComp->GetVelocity();
	if (box.min.x < 0.0f) // Left Wall
	{
		float rot = GetRotation();
		if (vel.x < 0.0f && vel.y < 0.0f) // Quadrant II direction (Clockwise)
		{
			rot -= (Math::PI * 0.5f);
		}
		else if (vel.x < 0.0f && vel.y > 0.0f) // Quadrant III direction (Counter-Clockwise)
		{
			rot += (Math::PI * 0.5f);
		}
		SetRotation(rot);

		Vector2D newPos = Vector2D(0.0f + halfSpriteWidth, GetPosition().y);
		SetPosition(newPos);
	}
	else if (box.max.x > WINDOW_WIDTH) // Right Wall
	{
		float rot = GetRotation();
		if (vel.x > 0.0f && vel.y > 0.0f) // Quadrant IV direction (Clockwise)
		{
			rot -= (Math::PI * 0.5f);
		}
		else if (vel.x > 0.0f && vel.y < 0.0f) // Quadrant I direction (Counter-Clockwise)
		{
			rot += (Math::PI * 0.5f);
		}
		SetRotation(rot);

		Vector2D newPos = Vector2D(WINDOW_WIDTH - halfSpriteWidth, GetPosition().y);
		SetPosition(newPos);
	}
	else if (box.min.y < 0.0f) // Top Wall
	{
		float rot = GetRotation();
		if (vel.x > 0.0f && vel.y < 0.0f) // Quadrant I direction (Clockwise)
		{
			rot -= (Math::PI * 0.5f);

		}
		else if (vel.x < 0.0f && vel.y < 0.0f) // Quadrant II direction (Counter-Clockwise)
		{
			rot += (Math::PI * 0.5f);
		}
		SetRotation(rot);

		Vector2D newPos = Vector2D(GetPosition().x, 0.0f + halfSpriteHeight);
		SetPosition(newPos);
	}
	else if (box.max.y > WINDOW_HEIGHT) // Bottom Wall
	{
		float rot = GetRotation();
		if (vel.x < 0.0f && vel.y > 0.0f) // Quadrant III direction (Clockwise)
		{
			rot -= (Math::PI * 0.5f);
		}
		else if (vel.x > 0.0f && vel.y > 0.0f) // Quadrant IV direction (Counter-Clockwise)
		{
			rot += (Math::PI * 0.5f);
		}
		SetRotation(rot);

		Vector2D newPos = Vector2D(GetPosition().x, WINDOW_HEIGHT - halfSpriteHeight);
		SetPosition(newPos);
	}
}

void ActorBall::RenderActorDebug(SDL_Renderer* renderer)
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