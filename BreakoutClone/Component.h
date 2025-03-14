#pragma once

class Component
{
public:
	// Lower update order = Earlier update
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const struct InputState& inputState) {}
	virtual void Update(float deltaTime) {}
	virtual void OnUpdateTransform() {}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;
	int mUpdateOrder;
};

