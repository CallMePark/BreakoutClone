#pragma once

#include "Component.h"
#include "Math.h"

#include <vector>

struct AABB
{
	Vector2D min; // Top left coordinate of box
	Vector2D max; // Bottom right coordinate of box

	AABB()
		: min(Vector2D::Zero), max(Vector2D::Zero)
	{}

	AABB(const Vector2D& inMin, const Vector2D& inMax)
		: min(inMin), max(inMax)
	{}

	float GetWidth() const
	{
		return max.x - min.x;
	}

	float GetHeight() const
	{
		return max.y - min.y;
	}

	Vector2D GetCenter() const
	{
		return Vector2D{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f };
	}
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(class Actor* owner, int updateOrder = 50);
	~CollisionComponent();

	void OnUpdateTransform() override;

	const AABB& GetWorldAABB() const { return mWorldAABB; }
	void SetObjectAABB(const AABB& inAABB) { mObjectAABB = inAABB; }

private:
	AABB mObjectAABB;
	AABB mWorldAABB;
};

