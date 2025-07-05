#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "camera.h"
#include "vector2.h"
#include "animation.h"
#include "playerId.h"
#include "platform.h"
#include "bullet.h"
#include "particle.h"

#include <graphics.h>

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern bool is_debug;

class Player
{
public:
	Player(bool facing_right = true) : is_facing_right(facing_right)
	{
		this->m_current_animaiton = this->is_facing_right ? &m_animation_idle_right : &m_animation_idle_left;

		this->m_current_animaiton = &this->m_animation_idle_right;

		this->m_timer_invulnerable.setWaitTime(750);
		this->m_timer_invulnerable.setOneShot(true);
		this->m_timer_invulnerable.setCallback([&]() { this->is_invulnerable = false; });

		this->m_timer_invulnerable_blink.setWaitTime(75);
		this->m_timer_invulnerable_blink.setCallback([&]() {this->is_show_sketch_frame = !this->is_show_sketch_frame; });

		this->m_timer_attack_cd.setWaitTime(this->m_attack_cd);
		this->m_timer_attack_cd.setOneShot(true);
		this->m_timer_attack_cd.setCallback([&]() {this->can_attack = true; });

		this->m_timer_run_effect_generation.setWaitTime(75);
		this->m_timer_run_effect_generation.setCallback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.getImage(0);
				particle_position.m_x = this->m_position.m_x + (this->m_size.m_x - frame->getwidth()) / 2;
				particle_position.m_y = this->m_position.m_y + this->m_size.m_y - frame->getheight();

				m_particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});

		this->m_timer_die_effect_generation.setWaitTime(35);
		this->m_timer_die_effect_generation.setCallback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.getImage(0);
				particle_position.m_x = this->m_position.m_x + (this->m_size.m_x - frame->getwidth()) / 2;
				particle_position.m_y = this->m_position.m_y + this->m_size.m_y - frame->getheight();

				m_particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		this->m_animation_jump_effect.setAtlas(&atlas_jump_effect);
		this->m_animation_jump_effect.setInterval(25);
		this->m_animation_jump_effect.setLoop(false);
		this->m_animation_jump_effect.setCallBack([&]() { this->is_jump_effect_visible = false; });

		this->m_animation_land_effect.setAtlas(&atlas_land_effect);
		this->m_animation_land_effect.setInterval(50);
		this->m_animation_land_effect.setLoop(false);
		this->m_animation_land_effect.setCallBack([&]() { this->is_land_effect_visible = false; });

		this->m_timer_cursor_visibility.setWaitTime(2500);
		this->m_timer_cursor_visibility.setOneShot(true);
		this->m_timer_cursor_visibility.setCallback([&]() {this->is_cursor_visible = false; });

	}

	~Player() = default;

	virtual void on_update(int delta)
	{
		if (this->is_attack_keydown)
		{
			if (this->can_attack)
			{
				this->on_attack();
				this->can_attack = false;
				this->m_timer_attack_cd.restart();
			}
		}

		int direction = this->is_right_keydown - this->is_left_keydown;

		if (direction != 0)
		{
			if (!this->is_attacking_ex) this->is_facing_right = direction > 0;

			this->m_current_animaiton = is_facing_right ? &this->m_animation_run_right : &this->m_animation_run_left;
			
			float distance = direction * this->run_velocity * delta;
			this->on_run(distance);
		}
		else
		{
			this->m_current_animaiton = is_facing_right ? &this->m_animation_idle_right : &this->m_animation_idle_left;
			this->m_timer_run_effect_generation.pause();
		}

		if (this->is_attacking_ex) this->m_current_animaiton = this->is_facing_right ? &this->m_animation_attack_ex_right : &this->m_animation_attack_ex_left;

		if (this->m_hp <= 0) this->m_current_animaiton = this->m_last_hurt_direction.m_x < 0 ? &this->m_animation_die_left : &this->m_animation_die_right;

		this->m_current_animaiton->on_update(delta);
		this->m_animation_jump_effect.on_update(delta);
		this->m_animation_land_effect.on_update(delta);

		this->m_timer_attack_cd.on_updata(delta);
		this->m_timer_invulnerable.on_updata(delta);
		this->m_timer_invulnerable_blink.on_updata(delta);
		
		this->m_timer_run_effect_generation.on_updata(delta);
		this->m_timer_cursor_visibility.on_updata(delta);

		if (this->m_hp <= 0) this->m_timer_die_effect_generation.on_updata(delta);

		this->m_particle_list.erase(std::remove_if(
		this->m_particle_list.begin(), this->m_particle_list.end(),
			[](const Particle& particle) 
			{
				return !particle.checkValid();
			}), this->m_particle_list.end());
		for (Particle& p : this->m_particle_list) p.on_update(delta);

		if (this->is_show_sketch_frame) sketchImage(this->m_current_animaiton->getFrame(), &this->m_img_sketch);

		this->moveAndCollide(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		for (const Particle& p : this->m_particle_list) p.on_draw(camera);

		if (this->is_jump_effect_visible) this->m_animation_jump_effect.on_draw(camera, (int)this->m_position_jump_effect.m_x, (int)this->m_position_jump_effect.m_y);
		if (this->is_land_effect_visible) this->m_animation_land_effect.on_draw(camera, (int)this->m_position_land_effect.m_x, (int)this->m_position_land_effect.m_y);

		if (this->m_hp > 0 && this->is_invulnerable && this->is_show_sketch_frame) putImageAlpha(camera, (int)this->m_position.m_x, (int)this->m_position.m_y, &this->m_img_sketch);
		else this->m_current_animaiton->on_draw(camera, this->m_position.m_x, this->m_position.m_y);

		if (this->is_cursor_visible)
		{
			switch (this->m_id)
			{
			case PlayerId::P1:
				putImageAlpha(camera, (int)(this->m_position.m_x + (this->m_size.m_x - img_1P_cursor.getwidth()) / 2), (int)(this->m_position.m_y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerId::P2:
				putImageAlpha(camera, (int)(this->m_position.m_x + (this->m_size.m_x - img_2P_cursor.getwidth()) / 2), (int)(this->m_position.m_y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}

		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)this->m_position.m_x, (int)this->m_position.m_y, (int)(this->m_position.m_x + this->m_size.m_x), (int)(this->m_position.m_y + this->m_size.m_y));
		}
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
				case 0x47://'G'
					if (this->m_mp >= 100) this->m_mp = 0, this->on_attack_ex();
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

	}

	void on_jump()
	{
		if (this->m_velocity.m_y != 0 || this->is_attacking_ex) return;

		this->m_velocity.m_y += this->jump_velocity;
		this->is_jump_effect_visible = true;
		this->m_animation_jump_effect.reset();

		IMAGE* effect_frame = this->m_animation_jump_effect.getFrame();
		this->m_position_jump_effect.m_x = this->m_position.m_x + (this->m_size.m_x - effect_frame->getwidth()) / 2;
		this->m_position_jump_effect.m_y = this->m_position.m_y + this->m_size.m_y - effect_frame->getheight();
	}

	void on_land()
	{
		this->is_land_effect_visible = true;
		this->m_animation_land_effect.reset();

		IMAGE* effect_frame = this->m_animation_land_effect.getFrame();
		this->m_position_land_effect.m_x = this->m_position.m_x + (this->m_size.m_x - effect_frame->getwidth()) / 2;
		this->m_position_land_effect.m_y = this->m_position.m_y + this->m_size.m_y - effect_frame->getheight();
	}

	void on_run(float destance) 
	{
		if (this->is_attacking_ex) return;

		this->m_position.m_x += destance;
		this->m_timer_run_effect_generation.resume();
	}

	void moveAndCollide(int delta)
	{
		float last_velocity_y = this->m_velocity.m_y;

		this->m_velocity.m_y += this->gravity * delta;
		this->m_position += this->m_velocity * (float)delta;

		if (this->m_hp <= 0) return;

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

						if (last_velocity_y != 0) this->on_land();

						break;
					}
				}
			}
		}

		if (!this->is_invulnerable)
		{
			for (Bullet* b : bullet_list)
			{
				if (!b->getValid() || b->getCollideTarget() != this->m_id) continue;

				if (b->checkCollide(this->m_position, this->m_size))
				{
					this->makeInvulnerable();

					b->on_collide();
					b->setValid(false);

					this->m_hp -= b->getDamage();
					this->m_last_hurt_direction = { b->getPosition().m_x - this->m_position.m_x, b->getPosition().m_y - this->m_position.m_y };
					if (this->m_hp <= 0)
					{
						this->m_velocity.m_x = this->m_last_hurt_direction.m_x < 0 ? 0.35f : -0.35f;
						this->m_velocity.m_y = -1.0f;
					}
				}
			}
		}
	}

	void makeInvulnerable()
	{
		this->is_invulnerable = true;
		this->m_timer_invulnerable.restart();
	}

	void setId(PlayerId id) { this->m_id = id; }

	void setPosition(int x, int y) { this->m_position.m_x = x, this->m_position.m_y = y; }

	const Vector2& getPosition() const { return this->m_position; }

	const Vector2& getSize() const { return this->m_size; }

	const int getHp() const { return this->m_hp; }

	void setHp(int hp) { this->m_hp = hp; }

	const int getMp() const { return this->m_mp; }

	virtual void on_attack() {}

	virtual void on_attack_ex() {}

protected:
	int m_hp = 100;
	int m_mp = 0;

	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;

	Vector2 m_position_jump_effect;
	Vector2 m_position_land_effect;

	Animation m_animation_idle_left;
	Animation m_animation_idle_right;
	Animation m_animation_run_left;
	Animation m_animation_run_right;
	Animation m_animation_attack_ex_left;
	Animation m_animation_attack_ex_right;
	Animation m_animation_jump_effect;
	Animation m_animation_land_effect;
	Animation m_animation_die_left;
	Animation m_animation_die_right;

	Animation* m_current_animaiton = nullptr;

	PlayerId m_id;

	IMAGE m_img_sketch;

	const float run_velocity = 0.55f;//水平移动速度
	const float gravity = 0.0016f;//重力加速度
	const float jump_velocity = -0.85f;//跳跃速度

	bool is_jump_effect_visible = false;
	bool is_land_effect_visible = false;

	bool is_invulnerable = false;
	bool is_show_sketch_frame = false;

	bool is_left_keydown = false;
	bool is_right_keydown = false;

	bool is_facing_right = true;

	bool is_attack_keydown = false;
	bool is_attacking_ex = false;

	bool is_cursor_visible = true;

	bool can_attack = true;
	int m_attack_cd = 500;

	Timer m_timer_cursor_visibility;
	Timer m_timer_attack_cd;
	Timer m_timer_invulnerable;
	Timer m_timer_invulnerable_blink;

	std::vector<Particle> m_particle_list;

	Timer m_timer_run_effect_generation;
	Timer m_timer_die_effect_generation;

	Vector2 m_last_hurt_direction;
};

#endif