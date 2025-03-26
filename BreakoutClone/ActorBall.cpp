#include "ActorBall.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "GridSpatialPartition.h"

#include <algorithm>

ActorBall::ActorBall(Game* game)
	: Actor(game)
{
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->SetTexture(GetGame()->GetTexture("Assets/Ball_Grey.png"));

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetMaxForwardSpeed(500.0f);
	mMoveComp->SetForwardSpeed(mMoveComp->GetMaxForwardSpeed());

	mCollisionComp = new CollisionComponent(this);
	float halfTexWidth = mSpriteComp->GetTextureWidth() * 0.5f;
	float halfTexHeight = mSpriteComp->GetTextureHeight() * 0.5f;
	Vector2D min{ -halfTexWidth, -halfTexHeight };
	Vector2D max{ halfTexWidth, halfTexHeight };
	AABB box{ min, max };
	mCollisionComp->SetObjectAABB(box);
}

void ActorBall::ResolveActorCollision()
{
	Vector2D pos = GetPosition();
	float rot = GetRotation();

	const AABB& box = mCollisionComp->GetWorldAABB();
	Vector2D vel = mMoveComp->GetVelocity();
	vel.x = Math::IsNearZero(vel.x) ? 0.0f : vel.x;
	vel.y = Math::IsNearZero(vel.y) ? 0.0f : vel.y;

	ResolveWallCollision(box, vel, pos, rot);
	ResolveBlockCollision(box, vel, pos, rot);

	SetRotation(rot);
	SetPosition(pos);
}

void ActorBall::ResolveWallCollision(const AABB& box, Vector2D& vel, Vector2D& pos, float& rot)
{
	if (box.min.x < 0.0f) // Left Wall (Collide from right)
	{
		rot = CalculateReflectionAngle(vel, Vector2D::Right);
		pos = Vector2D(0.0f + (box.GetWidth() * 0.5f), GetPosition().y);
	}
	else if (box.max.x > WINDOW_WIDTH) // Right Wall (Collide from left)
	{
		rot = CalculateReflectionAngle(vel, Vector2D::Left);
		pos = Vector2D(WINDOW_WIDTH - (box.GetWidth() * 0.5f), GetPosition().y);
	}
	else if (box.min.y < 0.0f) // Top Wall (Collide from down)
	{
		rot = CalculateReflectionAngle(vel, Vector2D::Down);
		pos = Vector2D(GetPosition().x, 0.0f + (box.GetHeight() * 0.5f));
	}
	else if (box.max.y > WINDOW_HEIGHT) // Bottom Wall (Collide from up)
	{
		rot = CalculateReflectionAngle(vel, Vector2D::Up);
		pos = Vector2D(GetPosition().x, WINDOW_HEIGHT - (box.GetHeight() * 0.5f));
	}
}

void ActorBall::ResolveBlockCollision(const AABB& box, Vector2D& vel, Vector2D& pos, float& rot)
{
	GridSpatialPartition* mGSP = GetGame()->GetGSP();
	std::vector<Cell> occupiedCells;

	// Resolve collision only if the cell is not empty
	if (!mGSP->IsInEmptyCell(this, occupiedCells))
	{
		// Find the cell closest to the actor
		Cell closestCell;
		float distToCell = 0;
		float upperBound = std::numeric_limits<float>::infinity();
		for (const Cell& cell : occupiedCells)
		{
			distToCell = pos.DistanceSqrTo(cell.box.GetCenter());

			if (distToCell < upperBound)
			{
				closestCell = cell;
				upperBound = distToCell;
			}
		}

		// Resolve collision with the cell member (block)
		const AABB& cellBox = closestCell.box;

		float overlapDepthX = std::min(box.max.x - cellBox.min.x, cellBox.max.x - box.min.x);
		float overlapDepthY = std::min(box.max.y - cellBox.min.y, cellBox.max.y - box.min.y);

		if (overlapDepthX < overlapDepthY) // Collision from left or right
		{
			if (box.min.x < cellBox.min.x) // Collision from Left
			{
				rot = CalculateReflectionAngle(vel, Vector2D::Left);
				pos = Vector2D(cellBox.min.x - (box.GetWidth() * 0.5f), GetPosition().y);;
			}
			else if (box.max.x > cellBox.max.x) // Collision from Right
			{
				rot = CalculateReflectionAngle(vel, Vector2D::Right);
				pos = Vector2D(cellBox.max.x + (box.GetWidth() * 0.5f), GetPosition().y);
			}
		}
		else if (overlapDepthX > overlapDepthY) // Top or Bottom of cell
		{
			if (box.min.y < cellBox.min.y) // Collision from Up
			{
				rot = CalculateReflectionAngle(vel, Vector2D::Up);
				pos = Vector2D(GetPosition().x, cellBox.min.y - (box.GetHeight() * 0.5f));
			}
			else if (box.max.y > cellBox.max.y) // Collision from Down
			{
				rot = CalculateReflectionAngle(vel, Vector2D::Down);
				pos = Vector2D(GetPosition().x, cellBox.max.y + (box.GetHeight() * 0.5f));
			}
		}

		closestCell.member->OnCollision();
	}
}

float ActorBall::CalculateReflectionAngle(const Vector2D& vel, const Vector2D& norm)
{
	/*
	* 1. Negate vel for vector projection
	* 2. Project vel onto norm using dot product
	* 3. Scale the projection by 2
	* 4. Add initial vel to the scaled projection for the reflected vector
	*	(Distributive property to simplify from V + 2(-V * N)N to V - 2(V * N)N 
	*/
	Vector2D reflectedVector = vel - (2.0f * (Vector2D::DotProduct(vel, norm)) * norm);
	reflectedVector.Normalize();

	// Convert unit vector to angle (radian)
	return std::atan2f(-reflectedVector.y, reflectedVector.x);
}

void ActorBall::RenderActorDebug(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	const AABB& box = mCollisionComp->GetWorldAABB();
	SDL_Rect debugBox = {
		static_cast<int>(box.min.x),
		static_cast<int>(box.min.y),
		static_cast<int>(box.max.x - box.min.x),
		static_cast<int>(box.max.y - box.min.y)
	};
	SDL_RenderDrawRect(renderer, &debugBox);
}