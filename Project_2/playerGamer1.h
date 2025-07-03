#ifndef _PLAYERGAMER1_H_
#define _PLAYERGAMER1_H_

#include "player.h"
#include "gamer1Bullet.h"

extern Atlas atlas_gamer1_idle_left;
extern Atlas atlas_gamer1_idle_right;
extern Atlas atlas_gamer1_run_left;
extern Atlas atlas_gamer1_run_right;
extern Atlas atlas_gamer1_attack_ex_left;
extern Atlas atlas_gamer1_attack_ex_right;

class PlayerGamer1 : public Player
{
public:
	PlayerGamer1()
	{
		this->m_animation_idle_left.setAtlas(&atlas_gamer1_idle_left);
		this->m_animation_idle_right.setAtlas(&atlas_gamer1_idle_right);
		this->m_animation_run_left.setAtlas(&atlas_gamer1_run_left);
		this->m_animation_run_right.setAtlas(&atlas_gamer1_run_right);
		this->m_animation_attack_ex_left.setAtlas(&atlas_gamer1_attack_ex_left);
		this->m_animation_attack_ex_right.setAtlas(&atlas_gamer1_attack_ex_right);

		this->m_animation_idle_left.setInterval(75);
		this->m_animation_idle_right.setInterval(75);
		this->m_animation_run_left.setInterval(75);
		this->m_animation_run_right.setInterval(75);
		this->m_animation_attack_ex_left.setInterval(75);
		this->m_animation_attack_ex_right.setInterval(75);

		this->m_size.m_x = 96;
		this->m_size.m_y = 96;

		this->m_timer_attack_ex.setWaitTime(this->attack_ex_duration);
		this->m_timer_attack_ex.setOneShot(true);
		this->m_timer_attack_ex.setCallback([&]() {this->is_attacking_ex = false; });

		this->m_timer_spawn_bullet_ex.setWaitTime(100);
		this->m_timer_spawn_bullet_ex.setOneShot(false);
		this->m_timer_spawn_bullet_ex.setCallback([&]() {this->spawnBullet1(this->speed_bullet_ex); });

		this->m_attack_cd = 100;
	}

	~PlayerGamer1() = default;

	virtual void on_update(int delta)
	{
		Player::on_update(delta);

		if (this->is_attacking_ex)
		{
			main_camera.shake(5, 100);
			this->m_timer_attack_ex.on_updata(delta);
			this->m_timer_spawn_bullet_ex.on_updata(delta);
		}
	}

	void spawnBullet1(float speed)
	{
		Bullet* bullet = new Gamer1Bullet();

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->getSize();

		bullet_position.m_x = this->is_facing_right ? this->m_position.m_x + this->m_size.m_x - bullet_size.m_x / 2 : this->m_position.m_x - bullet_size.m_x / 2;
		bullet_position.m_y = this->m_position.m_y;

		bullet_velocity.m_x = this->is_facing_right ? speed : -speed;
		bullet_velocity.m_y = 0;

		bullet->setPosition(bullet_position.m_x, bullet_position.m_y);
		bullet->setVelocity(bullet_velocity.m_x, bullet_velocity.m_y);

		bullet->setCollideTarget(this->m_id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
		bullet->setCallback([&]() {this->m_mp += 25; });

		bullet_list.push_back(bullet);
	}

	void on_attack()
	{
		this->spawnBullet1(this->speed_bullet);

		switch (rand() % 2)
		{
		case 0:
			mciSendString(L"player gamer1_bullet_shoot_1 from 0", NULL, 0, NULL);
			break;
		case 1:
			mciSendString(L"player gamer1_bullet_shoot_2 from 0", NULL, 0, NULL);
			break;
		}
	}

	void on_attack_ex()
	{
		this->is_attacking_ex = true;
		this->m_timer_attack_ex.restart();

		this->is_facing_right ? this->m_animation_attack_ex_right.reset() : this->m_animation_attack_ex_left.reset();

		mciSendString(L"player gamer1_bullet_shoot_ex from 0", NULL, 0, NULL);

	}

private:
	const float speed_bullet = 0.75f;
	const float speed_bullet_ex = 1.5f;
	const int attack_ex_duration = 2500;

	Timer m_timer_attack_ex;
	Timer m_timer_spawn_bullet_ex;

};

#endif // !_PLAYERGAMER1_H_
