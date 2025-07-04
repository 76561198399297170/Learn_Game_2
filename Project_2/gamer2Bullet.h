#ifndef _GAMER2_BULLET_H_
#define _GAMER2_BULLET_H_

#include "bullet.h"
#include "animation.h"

extern Camera main_camera;

extern Atlas atlas_gemer2_bullet;
extern Atlas atlas_gemer2_bullet_explode;

class Gamer2Bullet : public Bullet
{
public:
	Gamer2Bullet() 
	{
		this->m_size.m_x = this->m_size.m_y = 96;
		this->m_damage = 20;
		
		this->m_animation_idle.setAtlas(&atlas_gemer2_bullet);
		this->m_animation_idle.setInterval(50);

		this->m_animation_explode.setAtlas(&atlas_gemer2_bullet_explode);
		this->m_animation_explode.setInterval(50);
		this->m_animation_explode.setLoop(false);
		this->m_animation_explode.setCallBack([&]() {this->can_remove = true; });

		IMAGE* frame_idle = this->m_animation_idle.getFrame();
		IMAGE* frame_explode = this->m_animation_explode.getFrame();

		this->m_explode_render_offset.m_x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		this->m_explode_render_offset.m_y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;

	}

	~Gamer2Bullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(L"play gamer2_bullet_explode from 0", NULL, 0, NULL);
	}

	void on_draw(const Camera& camera) const
	{
		if (this->is_valid) 
		{
			this->m_animation_idle.on_draw(camera, this->m_position.m_x, this->m_position.m_y);
		}
		else
		{
			this->m_animation_explode.on_draw(camera, this->m_position.m_x + this->m_explode_render_offset.m_x, this->m_position.m_y + this->m_explode_render_offset.m_y);
		}
		Bullet::on_draw(camera);
	}

	void on_update(int delta)
	{
		if (this->is_valid)
		{
			this->m_velocity.m_y += this->m_gravity * delta;
			this->m_position += this->m_velocity * (float)delta;
		}

		if (!this->is_valid)
		{
			this->m_animation_explode.on_update(delta);
		}
		else
		{
			this->m_animation_idle.on_update(delta);
		}

		if (this->checkIfExceedScreen())
		{
			this->can_remove = true;
		}
	}

private:
	const float m_gravity = 0.001f;

	Animation m_animation_idle;
	Animation m_animation_explode;
	Vector2 m_explode_render_offset;

};

#endif