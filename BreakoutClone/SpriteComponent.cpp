#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

#include <numbers>

SpriteComponent::SpriteComponent(Actor* actor, int drawOrder)
	: Component(actor), mTexture(nullptr), mDrawOrder(drawOrder), mTextureWidth(0), mTextureHeight(0)
{
	mOwner->GetGame()->AddSpriteComponent(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSpriteComponent(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect rect;

		// Scale the texture width/height by the owner's scale
		rect.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
		rect.h = static_cast<int>(mTextureHeight * mOwner->GetScale());

		// Align rect center to owner's position (current: rect top-left = owner position)
		rect.x = static_cast<int>(mOwner->GetPosition().x - rect.w / 2);
		rect.y = static_cast<int>(mOwner->GetPosition().y - rect.h / 2);

		// Use RenderCopyEx if texture rotation/flip is required
		SDL_RenderCopy(renderer, mTexture, nullptr, &rect);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// Set texture width and height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}
