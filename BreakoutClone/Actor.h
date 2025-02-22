#pragma once

#include <cstdint>
#include <vector>

class Actor
{
public:
	enum State { EActive, EPaused, EDead };
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	Actor(class Game* game);
	virtual ~Actor();

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState) {}

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime) {}

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;
	class Game* mGame;

	std::vector<class Component*> mComponents;
};

