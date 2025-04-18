#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "timer.h"
#include "vector2.h"

class Camera
{
public:
	Camera()
	{
		this->m_shake_timer.setOneShot(true);
		this->m_shake_timer.setCallback(
			[&]()
			{
				this->is_shaking = false;
				this->reset();
			}
		);
	}

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
		this->m_shake_timer.on_updata(delta);

		if (this->is_shaking)
		{
			float radius = this->m_shaking_strength;
			float angle = (float)(rand() % 360) * 3.141592f / 180.0f;

			this->m_position.m_x = radius * cos(angle);
			this->m_position.m_y = radius * sin(angle);
		}
	}

	void shake(float strength, int duration)
	{
		this->is_shaking = true;
		this->m_shaking_strength = strength;

		this->m_shake_timer.setWaitTime(duration);
		this->m_shake_timer.restart();
	}

private:
	Vector2 m_position;
	
	Timer m_shake_timer;
	bool is_shaking = false;
	float m_shaking_strength = 0;

};

#endif