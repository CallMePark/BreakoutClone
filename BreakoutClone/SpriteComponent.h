#pragma once

#include "Component.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* actor, int drawOrder = 100);
	~SpriteComponent(); // Will be invoked before Component(parent) destructor

	void Draw(struct SDL_Renderer* renderer);
	void SetTexture(struct SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }

private:
	struct SDL_Texture* mTexture;
	int mDrawOrder;
	int mTextureWidth;
	int mTextureHeight;
};

