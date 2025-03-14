#include "Actor.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>

Actor::Actor(Game* game)
	: mGame(game), mState(EActive), mPosition(Vector2D::Zero), mScale(1.0f), mRotation(0.0f),
	mIsUpdateTransform(false)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);

	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::ProcessInput(const InputState& inputState)
{
	if (mState == EActive)
	{
		for (auto component : mComponents)
		{
			component->ProcessInput(inputState);
		}
		ProcessActorInput(inputState);
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		OnUpdateTransform(); // Preps transform for the following updates

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		OnUpdateTransform(); // Preps transform for any post-update transform update
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void Actor::OnUpdateTransform()
{
	if (mIsUpdateTransform)
	{
		mIsUpdateTransform = false;

		for (auto component : mComponents)
		{
			component->OnUpdateTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	// Insert component based on its update order (lower order updates first)
	int newUpdateOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter)
	{
		if (newUpdateOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	// This is invoked when component is deleted in Actor::~Actor, so just erase from vector
	// To maintain component update order, do not attempt to optimize with iter_swap and pop_back
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}