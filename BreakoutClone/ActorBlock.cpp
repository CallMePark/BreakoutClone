#include "ActorBlock.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"


ActorBlock::ActorBlock(Game* game)
	: Actor(game)
{
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->SetTexture(GetGame()->GetTexture("Assets/Block_Blue.png"));

	mCollisionComp = new CollisionComponent(this);
	float texHeightHalf = mSpriteComp->GetTextureHeight() * 0.5f;
	float texWidthHalf = mSpriteComp->GetTextureWidth() * 0.5f;
	Vector2D min{ -texWidthHalf, -texHeightHalf };
	Vector2D max{ texWidthHalf, texHeightHalf };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);

	SetScale(1.0f);
	SetPosition(Vector2D(32.0f + (64.0f * 3), 16.0f + (32.0f * 3)));
}

void ActorBlock::RenderActorDebug(SDL_Renderer* renderer)
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
