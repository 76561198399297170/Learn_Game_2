#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y) : m_x(x), m_y(y) {}

	Vector2 operator+(const Vector2& vec)
	{
		return Vector2(this->m_x + vec.m_x, this->m_y + vec.m_y);
	}

	void operator+=(const Vector2& vec)
	{
		this->m_x += vec.m_x, this->m_y += vec.m_y;
	}

	Vector2 operator-(const Vector2& vec)
	{
		return Vector2(this->m_x - vec.m_x, this->m_y - vec.m_y);
	}

	void operator-=(const Vector2& vec)
	{
		this->m_x -= vec.m_x, this->m_y -= vec.m_y;
	}

	float operator*(const Vector2& vec)
	{
		return this->m_x * vec.m_x + this->m_y * vec.m_y;
	}

	Vector2 operator*(const float val) const
	{
		return Vector2(this->m_x * val, this->m_y * val);
	}

	void operator*=(float val)
	{
		this->m_x *= val, this->m_y *= val;
	}

	float length()
	{
		return sqrt(pow(this->m_x, 2) + pow(this->m_y, 2));
	}

	Vector2 normalize()
	{
		float len = this->length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(this->m_x / len, this->m_y / len);
	}

public:
	float m_x;
	float m_y;

};

#endif