#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mRotationSpeed(0.0f), mMaxRotationSpeed(0.0f), 
	mForwardSpeed(0.0f), mMaxForwardSpeed(0.0f)
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

	if (!Math::IsNearZero(mForwardSpeed))
	{
		Vector2D pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
