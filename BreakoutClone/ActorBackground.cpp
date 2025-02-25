#include "ActorBackground.h"
#include "SpriteComponent.h"
#include "Game.h"

ActorBackground::ActorBackground(Game* game)
	: Actor(game)
{
	SpriteComponent* bgSprite = new SpriteComponent(this);
	bgSprite->SetTexture(GetGame()->GetTexture("Assets/BGLevel1.png"));

	SetPosition(Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}