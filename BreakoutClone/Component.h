#pragma once

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const struct InputState& inputState) {}
	virtual void Update(float deltaTime) {}

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;

private:
	int mUpdateOrder;
};

