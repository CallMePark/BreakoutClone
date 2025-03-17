#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mRotationSpeed(0.0f), mMaxRotationSpeed(0.0f), 
	mForwardSpeed(0.0f), mMaxForwardSpeed(0.0f), mCurrentVelocity(Vector2D::Zero)
{}

void MoveComponent::Update(float deltaTime)
{
	// Rotate first before moving position
	if (!Math::IsNearZero(mRotationSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mRotationSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	mCurrentVelocity = Vector2D::Zero;
	if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector2D pos = mOwner->GetPosition();

		mCurrentVelocity = mOwner->GetForward() * mForwardSpeed;

		pos += mCurrentVelocity * deltaTime;
		mOwner->SetPosition(pos);
	}
}
