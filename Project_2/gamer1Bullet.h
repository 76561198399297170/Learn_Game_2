#ifndef _GAMER1_BULLET_H_
#define _GAMER1_BULLET_H_

#include "bullet.h"
#include "animation.h"

extern IMAGE img_gamer1_bullet;
extern Atlas atlas_gamer1_bullet_break;

class Gamer1Bullet : public Bullet
{
public:
	Gamer1Bullet() 
	{
		this->m_size.m_y = this->m_size.m_x = 64;
		this->m_damage = 10;

		this->m_animation_bullet_break.setAtlas(&atlas_gamer1_bullet_break);
		this->m_animation_bullet_break.setInterval(100);
		this->m_animation_bullet_break.setLoop(false);
		this->m_animation_bullet_break.setCallBack([&]() {this->can_remove = true; });
	}

	~Gamer1Bullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		switch (rand() % 3)
		{
		case 0:
			mciSendString(L"play gamer1_bullet_break_1 from 0", NULL, 0, NULL);
			break;
		case 1:
			mciSendString(L"play gamer1_bullet_break_2 from 0", NULL, 0, NULL);
			break;
		case 2:
			mciSendString(L"play gamer1_bullet_break_3 from 0", NULL, 0, NULL);
			break;
		default:
			break;
		}
	}

	void on_draw(const Camera& camera) const
	{
		if (this->can_remove)
		{
			putImageAlpha(camera, (int)this->m_position.m_x, (int)this->m_position.m_y, &img_gamer1_bullet);
		}
		else
		{
			this->m_animation_bullet_break.on_draw(camera, (int)this->m_position.m_x, (int)this->m_position.m_y);
		}
	}

	void on_update(int delta)
	{
		this->m_position += this->m_velocity * (float)delta;

		if (this->is_valid)
		{
			this->m_animation_bullet_break.on_update(delta);
		}

		if (this->checkIfExceedScreen())
		{
			this->can_remove = true;
		}
	}

private:
	Animation m_animation_bullet_break;

};

#endif