#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "camera.h"
#include "vector2.h"
#include "animation.h"
#include "playerId.h"

#include <graphics.h>

class Player
{
public:
	Player()
	{
		this->m_current_animaiton = &this->m_animation_idle_right;
	}

	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = this->is_right_keydown - this->is_left_keydown;

		if (direction != 0)
		{
			this->is_facing_right = direction > 0;
			this->m_current_animaiton = is_facing_right ? &this->m_animation_run_right : &this->m_animation_run_left;
		}
		else
		{
			this->m_current_animaiton = is_facing_right ? &this->m_animation_idle_right : &this->m_animation_idle_left;
		}

		this->m_current_animaiton->on_update(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		this->m_current_animaiton->on_draw(camera, this->m_position.m_x, this->m_position.m_y);
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (this->m_id)
			{
			case PlayerId::P1:
				switch (msg.vkcode)
				{
				case 0x41://'A'
					this->is_left_keydown = true;
					break;
				case 0x44://'D'
					this->is_right_keydown = true;
					break;
				}
				break;
			case PlayerId::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://'¡û'
					this->is_left_keydown = true;
					break;
				case VK_RIGHT://'¡ú'
					this->is_right_keydown = true;
					break;
				}
				break;
			}
			break;
		case WM_KEYUP:
			switch (this->m_id)
			{
			case PlayerId::P1:
				switch (msg.vkcode)
				{
				case 0x41://'A'
					this->is_left_keydown = false;
					break;
				case 0x44://'D'
					this->is_right_keydown = false;
					break;
				}
				break;
			case PlayerId::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://'¡û'
					this->is_left_keydown = false;
					break;
				case VK_RIGHT://'¡ú'
					this->is_right_keydown = false;
					break;
				}
				break;
			}
			break;
		}
	}

	void setId(PlayerId id) { this->m_id = id; }

	void setPosition(int x, int y) { this->m_position.m_x = x, this->m_position.m_y = y; }

protected:
	Vector2 m_position;

	Animation m_animation_idle_left;
	Animation m_animation_idle_right;
	Animation m_animation_run_left;
	Animation m_animation_run_right;

	Animation* m_current_animaiton = nullptr;

	PlayerId m_id;

	bool is_left_keydown = false;
	bool is_right_keydown = false;

	bool is_facing_right = true;

};


#endif // !_PLAYER_H_
