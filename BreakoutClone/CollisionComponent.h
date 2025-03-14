#pragma once

#include "Component.h"
#include "Math.h"

#include <vector>

struct AABB
{
	Vector2D min; // Top left coordinate of box
	Vector2D max; // Bottom right coordinate of box

	AABB(const Vector2D& inMin, const Vector2D& inMax)
		: min(inMin), max(inMax)
	{}

	Vector2D GetCenter() const
	{
		return Vector2D{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f };
	}

	std::vector<Vector2D> GetCorners() const
	{
		return {
			Vector2D{min.x, min.y},
			Vector2D{max.x, min.y},
			Vector2D{min.x, max.y},
			Vector2D{max.x, max.y}
		};
	}
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(class Actor* owner, int updateOrder = 50);
	~CollisionComponent();

	void AddToGSP();
	void OnUpdateTransform() override;

	const AABB& GetWorldAABB() const { return mWorldAABB; }
	void SetObjectAABB(const AABB& inAABB) { mObjectAABB = inAABB; }

private:
	class GridSpatialPartition* mGSP;

	AABB mObjectAABB;
	AABB mWorldAABB;
};

