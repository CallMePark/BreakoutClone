#pragma once

#include <cmath>

namespace Math
{
	constexpr float PI = 3.1415927f;
	constexpr float DEGREES_PER_RADIAN = 180.0f / PI;
	constexpr float RADIANS_PER_DEGREE = PI / 180.0f;

	inline float ToDegree(float radian) { return radian * DEGREES_PER_RADIAN; }
	inline float ToRadian(float degree) { return degree * RADIANS_PER_DEGREE; }

	inline bool IsNearZero(float value, float epsilon = 0.001f)
	{
		return fabs(value) < epsilon;
	}
}

class Vector2D
{
public:
	float x, y;

	Vector2D()
		: x(0.0f), y(0.0f)
	{}

	explicit Vector2D(float inX, float inY)
		: x(inX), y(inY)
	{}

	bool IsNearZero(float epsilon = 0.001f) const
	{
		return (fabs(x) < epsilon) && (fabs(y) < epsilon);
	}

	float DistanceSqrTo(const Vector2D& other) const
	{
		return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
	}

	Vector2D operator+(const Vector2D& other)
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator*(float scalar)
	{
		return Vector2D(x * scalar, y * scalar);
	}

	Vector2D& operator+=(const Vector2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2D& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	bool operator==(const Vector2D& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	static const Vector2D Zero;
	static const Vector2D Right;
	static const Vector2D Left;
};