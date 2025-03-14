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

	//mMoveComp = new MoveComponent(this);
	//mMoveComp->SetDefaultSpeed(50.0f);

	mCollisionComp = new CollisionComponent(this);
	float texHeightHalf = mSpriteComp->GetTextureHeight() * 0.5f;
	float texWidthHalf = mSpriteComp->GetTextureWidth() * 0.5f;
	Vector2D min{ -texWidthHalf, -texHeightHalf };
	Vector2D max{ texWidthHalf, texHeightHalf };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);

	SetPosition(Vector2D(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 100.0f));
	
	//float startDirDegree = 60.f;
	//float startDirRadian = Math::ToRadian(startDirDegree);
	//mMoveComp->SetMoveDir(Vector2D(cos(startDirRadian), -sin(startDirRadian)));
}

void ActorBall::UpdateActor(float deltaTime)
{
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

float ActorBall::ClampPositionToScreenEdge(float inPosX)
{
	return 0.0f;
}
