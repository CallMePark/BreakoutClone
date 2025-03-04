#pragma once

namespace Math
{
	constexpr float PI = 3.1415927f;
	constexpr float DEGREES_PER_RADIAN = 180.0f / PI;
	constexpr float RADIANS_PER_DEGREE = PI / 180.0f;

	inline float ToDegree(float radian) { return radian * DEGREES_PER_RADIAN; }
	inline float ToRadian(float degree) { return degree * RADIANS_PER_DEGREE; }
}

struct Vector2D
{
	float x, y;

	Vector2D()
		: x(0.0f), y(0.0f)
	{}

	Vector2D(float inX, float inY)
		: x(inX), y(inY)
	{}
};