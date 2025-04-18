#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector2.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void reset()
	{
		this->m_position.m_y = this->m_position.m_x = 0;
	}

	const Vector2& getPosition() const
	{
		return this->m_position;
	}

	void on_updata(int delta)
	{
		const Vector2 speed = { -0.35f, 0 };
		this->m_position += speed * (float)delta;
	}

private:
	Vector2 m_position;

};

#endif