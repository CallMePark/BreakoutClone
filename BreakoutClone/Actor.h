#pragma once

#include "Math.h"

#include <vector>

class Actor
{
public:
	enum State { EActive, EPaused, EDead };

	Actor(class Game* game);
	virtual ~Actor();

	// Game loop
	void ProcessInput(const struct InputState& inputState);
	void Update(float deltaTime);
	virtual void RenderActorDebug(struct SDL_Renderer* renderer) {}

	// Own/manage components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	class Game* GetGame() const { return mGame; }
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	// Actor transform
	const Vector2D& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2D& inPosition) { mPosition = inPosition; mIsUpdateTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float inScale) { mScale = inScale; mIsUpdateTransform = true; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float inRotation) { mRotation = inRotation; mIsUpdateTransform = true; }

	// Negate sine to account for SDL Window's +y pointing downwards
	Vector2D GetForward() const { return Vector2D(cosf(mRotation), -sinf(mRotation)); }

	// Null by default. Will be assigned in CollisionComponent constructor
	class CollisionComponent* GetCollisionComponent() const { return mCollisionComp; }
	void SetCollisionComponent(class CollisionComponent* inCollisionComp) { mCollisionComp = inCollisionComp; }

	virtual void OnCollision() {}

protected:
	virtual void ProcessActorInput(const struct InputState& inputState) {}
	virtual void UpdateActor(float deltaTime) {}
	virtual void ResolveActorCollision() {};
	

	class Game* mGame;
	State mState;
	class CollisionComponent* mCollisionComp;

private:
	void UpdateComponents(float deltaTime);
	void OnUpdateTransform();

	std::vector<class Component*> mComponents;
	
	// Actor transform
	Vector2D mPosition;
	float mScale;
	float mRotation;
	bool mIsUpdateTransform;
};

