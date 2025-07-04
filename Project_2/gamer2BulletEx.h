#ifndef _GAMER2_BULLET_EX_H_
#define _GAMER2_BULLET_EX_H_

#include "bullet.h"
#include "animation.h"

extern Camera main_camera;

extern Atlas atlas_gamer2_bullet_ex;
extern Atlas atlas_gamer2_bullet_ex_explode;

class Gamer2BulletEx : public Bullet
{
public:
	Gamer2BulletEx()
	{
		this->m_size.m_x = this->m_size.m_y = 288;
		this->m_damage = 20;

		this->m_animation_idle.setAtlas(&atlas_gamer2_bullet_ex);
		this->m_animation_idle.setInterval(50);

		this->m_animation_explode.setAtlas(&atlas_gamer2_bullet_ex_explode);
		this->m_animation_explode.setInterval(50);
		this->m_animation_explode.setLoop(false);
		this->m_animation_explode.setCallBack([&]() {this->can_remove = true; });

		IMAGE* frame_idle = this->m_animation_idle.getFrame();
		IMAGE* frame_explode = this->m_animation_explode.getFrame();

		this->m_explode_render_offset.m_x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		this->m_explode_render_offset.m_y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;

	}

	~Gamer2BulletEx() = default;

	bool checkCollide(const Vector2& position, const Vector2& size)
	{
		bool is_collide_x = (max(this->m_position.m_x + this->m_size.m_x, position.m_x + size.m_x) - min(this->m_position.m_x, position.m_x) <= this->m_size.m_x + size.m_x);
		bool is_collide_y = (max(this->m_position.m_y + this->m_size.m_y, position.m_y + size.m_y) - min(this->m_position.m_y, position.m_y) <= this->m_size.m_y + size.m_y);

		return is_collide_x && is_collide_y;
	}

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(20, 250);

		mciSendString(L"play gamer2_bullet_explode_ex from 0", NULL, 0, NULL);
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
	Animation m_animation_idle;
	Animation m_animation_explode;
	Vector2 m_explode_render_offset;

};

#endif