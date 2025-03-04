#include "Actor.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>

Actor::Actor(Game* game)
	: mGame(game), mState(EActive), mPosition(Vector2D(0.0f, 0.0f)), mScale(1.0f), mRotation(0.0f)
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
		ActorInput(inputState);
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void Actor::AddComponent(Component* component)
{
	// Insert component based on updateorder (lower order updates first)
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
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}