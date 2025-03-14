#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "GridSpatialPartition.h"

CollisionComponent::CollisionComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mObjectAABB(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f)),
	mWorldAABB(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f))
{
	// Add this as cell member to grid
	mGSP = mOwner->GetGame()->GetGSP();
}

CollisionComponent::~CollisionComponent()
{
	// Remove owner from grid
	mGSP->RemoveCellMember(this);
}

void CollisionComponent::AddToGSP()
{
	mOwner->GetGame()->GetGSP()->AddCellMember(this);
}

void CollisionComponent::OnUpdateTransform()
{
	mWorldAABB = mObjectAABB;

	mWorldAABB.min *= mOwner->GetScale();
	mWorldAABB.max *= mOwner->GetScale();

	mWorldAABB.min += mOwner->GetPosition();
	mWorldAABB.max += mOwner->GetPosition();
}
