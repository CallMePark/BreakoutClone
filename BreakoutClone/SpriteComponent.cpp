#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

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

		// Use RenderCopy if texture rotation/flip is not required
		float pi = 3.1415927f;
		float rotDeg = mOwner->GetRotation() * 180.0f / pi;
		SDL_RenderCopyEx(
			renderer, 
			mTexture, 
			nullptr, 
			&rect, 
			Math::ToDegree(mOwner->GetRotation()), // Angle in degree, rotates clockwise
			nullptr, 
			SDL_FLIP_NONE
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// Set texture width and height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}