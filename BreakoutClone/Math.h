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

	float Length()
	{
		return std::sqrt((x * x) + (y * y));
	}

	void Normalize()
	{
		float length = this->Length();
		if (!Math::IsNearZero(length))
		{
			x /= length;
			y /= length;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
		}
	}

	static float DotProduct(const Vector2D& left, const Vector2D& right)
	{
		return (left.x * right.x) + (left.y * right.y);
	}

	friend Vector2D operator+(const Vector2D& left, const Vector2D& right)
	{
		return Vector2D(left.x + right.x, left.y + right.y);
	}

	friend Vector2D operator-(const Vector2D& left, const Vector2D& right)
	{
		return Vector2D(left.x - right.x, left.y - right.y);
	}

	friend Vector2D operator*(const Vector2D& left, float scalar)
	{
		return Vector2D(left.x * scalar, left.y * scalar);
	}

	friend Vector2D operator*(float scalar, const Vector2D& right)
	{
		return Vector2D(right.x * scalar, right.y * scalar);
	}

	friend bool operator==(const Vector2D& left, const Vector2D& right)
	{
		return (left.x == right.x) && (left.y == right.y);
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


	static const Vector2D Zero;
	static const Vector2D Right;
	static const Vector2D Left;
	static const Vector2D Up;
	static const Vector2D Down;
};