#pragma once

#include "Math.h"

#include <vector>

class Actor
{
public:
	enum State { EActive, EPaused, EDead };

	Actor(class Game* game);
	virtual ~Actor();

	void ProcessInput(const struct InputState& inputState);
	virtual void ActorInput(const struct InputState& inputState) {}

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime) {}

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// [START] Getters/Setters
	class Game* GetGame() const { return mGame; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	const Vector2D& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2D& inPosition) { mPosition = inPosition; }
	float GetScale() const { return mScale; }
	void SetScale(float inScale) { mScale = inScale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float inRotation) { mRotation = inRotation; }
	// [END] Getters/Setters

private:
	std::vector<class Component*> mComponents;
	
	class Game* mGame;
	State mState;

	Vector2D mPosition;
	float mScale;
	float mRotation;
};

