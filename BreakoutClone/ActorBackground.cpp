#include "ActorBackground.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

ActorBackground::ActorBackground(Game* game)
	: Actor(game)
{
	SpriteComponent* spriteComp = new SpriteComponent(this);
	spriteComp->SetTexture(GetGame()->GetTexture("Assets/Background_Level1.png"));

	mCollisionComp = new CollisionComponent(this);
	float texHeightHalf = spriteComp->GetTextureHeight() * 0.5f;
	float texWidthHalf = spriteComp->GetTextureWidth() * 0.5f;
	Vector2D min{ -texWidthHalf, -texHeightHalf };
	Vector2D max{ texWidthHalf, texHeightHalf };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);

	SetPosition(Vector2D(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}

void ActorBackground::RenderActorDebug(SDL_Renderer* renderer)
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
