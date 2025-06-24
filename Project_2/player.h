#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "camera.h"
#include "vector2.h"
#include "animation.h"
#include "playerId.h"
#include "platform.h"

#include <graphics.h>

extern std::vector<Platform> platform_list;

class Player
{
public:
	Player()
	{
		this->m_current_animaiton = &this->m_animation_idle_right;

		this->m_timer_attack_cd.setWaitTime(this->m_attack_cd);
		this->m_timer_attack_cd.setOneShot(true);
		this->m_timer_attack_cd.setCallback([&]() {this->can_attack = true; });
	}

	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = this->is_right_keydown - this->is_left_keydown;

		if (direction != 0)
		{
			this->is_facing_right = direction > 0;
			this->m_current_animaiton = is_facing_right ? &this->m_animation_run_right : &this->m_animation_run_left;
			
			float distance = direction * this->run_velocity * delta;
			this->on_run(distance);

		}
		else
		{
			this->m_current_animaiton = is_facing_right ? &this->m_animation_idle_right : &this->m_animation_idle_left;
		}

		this->m_current_animaiton->on_update(delta);
		this->m_timer_attack_cd.on_updata(delta);
		this->moveAndCollide(delta);
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
				case 0x57://'W'
					this->on_jump();
					break;
				case 0x46://'F'
					this->is_attack_keydown = true;
					break;
				}
				break;
			case PlayerId::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://'←'
					this->is_left_keydown = true;
					break;
				case VK_RIGHT://'→'
					this->is_right_keydown = true;
					break;
				case VK_UP://'↑'
					this->on_jump();
					break;
				case VK_OEM_PERIOD://'.'
					this->is_attack_keydown = true;
					break;
				case VK_OEM_COMMA://','
					if (this->m_mp >= 100) this->m_mp = 0, this->on_attack_ex();
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
				case 0x46://'F'
					this->is_attack_keydown = false;
					break;
				case 0x47:///'G'
					if (this->m_mp >= 100) this->m_mp = 0, this->on_attack_ex();
					break;
				}
				break;
			case PlayerId::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT://'←'
					this->is_left_keydown = false;
					break;
				case VK_RIGHT://'→'
					this->is_right_keydown = false;
					break;
				case VK_OEM_PERIOD://'.'
					this->is_attack_keydown = false;
					break;
				}
				break;
			}
			break;
		}

		if (this->is_attack_keydown)
		{
			if (this->can_attack)
			{
				this->on_attack();
				this->can_attack = false;
				this->m_timer_attack_cd.restart();
			}
		}
	}

	void on_jump()
	{
		if (this->m_velocity.m_y != 0) return;

		this->m_velocity.m_y += this->jump_velocity;
	}

	void on_run(float destance) 
	{
		this->m_position.m_x += destance;
	}

	void moveAndCollide(int delta)
	{
		this->m_velocity.m_y += this->gravity * delta;
		this->m_position += this->m_velocity * (float)delta;

		if (this->m_velocity.m_y > 0)
		{
			for (const Platform& pt : platform_list)
			{
				const Platform::CollisionShape& shape = pt.m_shape;

				bool is_collide_x = (max(this->m_position.m_x + this->m_size.m_x, shape.right) - min(this->m_position.m_x, shape.left) <= this->m_size.m_x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= this->m_position.m_y && shape.y <= this->m_position.m_y + this->m_size.m_y);

				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = this->m_velocity.m_y * delta;
					float last_tick_foot_pos_y = this->m_position.m_y + this->m_size.m_y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y)
					{
						this->m_position.m_y = shape.y - this->m_size.m_y;
						this->m_velocity.m_y = 0;

						break;
					}
				}
			}
		}
	}

	void setId(PlayerId id) { this->m_id = id; }

	void setPosition(int x, int y) { this->m_position.m_x = x, this->m_position.m_y = y; }

	virtual void on_attack() {}

	virtual void on_attack_ex() {}

protected:
	int m_hp = 100;
	int m_mp = 0;

	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;

	Animation m_animation_idle_left;
	Animation m_animation_idle_right;
	Animation m_animation_run_left;
	Animation m_animation_run_right;

	Animation* m_current_animaiton = nullptr;

	PlayerId m_id;

	const float run_velocity = 0.55f;//水平移动速度
	const float gravity = 0.0016f;//重力加速度
	const float jump_velocity = -0.85f;//跳跃速度

	bool is_left_keydown = false;
	bool is_right_keydown = false;

	bool is_facing_right = true;

	bool is_attack_keydown = false;
	bool can_attack = true;
	int m_attack_cd = 500;
	Timer m_timer_attack_cd;

};

#endif
