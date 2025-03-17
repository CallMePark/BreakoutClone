#pragma once

#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetRotationSpeed() const { return mRotationSpeed; }
	void SetRotationSpeed(float speed) { mRotationSpeed = speed; }
	float GetMaxRotationSpeed() const { return mMaxRotationSpeed; }
	void SetMaxRotationSpeed(float speed) { mMaxRotationSpeed = speed; }

	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }

	const Vector2D& GetVelocity() const { return mCurrentVelocity; }

private:
	float mRotationSpeed; // radians/second
	float mMaxRotationSpeed;
	float mForwardSpeed; // units/second
	float mMaxForwardSpeed;

	Vector2D mCurrentVelocity;
};

