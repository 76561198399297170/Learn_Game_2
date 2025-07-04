#ifndef _PLAYERGAMER2_H_
#define _PLAYERGAMER2_H_

#include "player.h"
#include "gamer2Bullet.h"
#include "gamer2BulletEx.h"

extern Atlas atlas_gamer2_idle_left;
extern Atlas atlas_gamer2_idle_right;
extern Atlas atlas_gamer2_run_left;
extern Atlas atlas_gamer2_run_right;
extern Atlas atlas_gamer2_attack_ex_left;
extern Atlas atlas_gamer2_attack_ex_right;
extern Atlas atlas_gamer2_bullet_text;

extern Player* player_1;
extern Player* player_2;

class PlayerGamer2 : public Player
{
public:
	PlayerGamer2()
	{
		this->m_animation_idle_left.setAtlas(&atlas_gamer2_idle_left);
		this->m_animation_idle_right.setAtlas(&atlas_gamer2_idle_right);
		this->m_animation_run_left.setAtlas(&atlas_gamer2_run_left);
		this->m_animation_run_right.setAtlas(&atlas_gamer2_run_right);
		this->m_animation_attack_ex_left.setAtlas(&atlas_gamer2_attack_ex_left);
		this->m_animation_attack_ex_right.setAtlas(&atlas_gamer2_attack_ex_right);
		this->m_animation_bullet_text.setAtlas(&atlas_gamer2_bullet_text);

		this->m_animation_idle_left.setInterval(75);
		this->m_animation_idle_right.setInterval(75);
		this->m_animation_run_left.setInterval(75);
		this->m_animation_run_right.setInterval(75);
		this->m_animation_attack_ex_left.setInterval(100);
		this->m_animation_attack_ex_right.setInterval(100);
		this->m_animation_bullet_text.setInterval(100);

		this->m_animation_attack_ex_left.setLoop(false);
		this->m_animation_attack_ex_right.setLoop(false);
		this->m_animation_bullet_text.setLoop(false);

		this->m_animation_attack_ex_left.setCallBack([&]() { this->is_bullet_ex_visible = this->is_attacking_ex = false; });
		this->m_animation_attack_ex_right.setCallBack([&]() { this->is_bullet_ex_visible = this->is_attacking_ex = false; });

		this->m_size.m_x = 96;
		this->m_size.m_y = 96;

		this->m_attack_cd = 250;
	}

	~PlayerGamer2() = default;

	virtual void on_update(int delta)
	{
		Player::on_update(delta);

		if (this->is_bullet_ex_visible) this->m_animation_bullet_text.on_update(delta);
	}
	
	virtual void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);

		if (this->is_bullet_ex_visible)
		{
			Vector2 text_position;
			IMAGE* frame = this->m_animation_bullet_text.getFrame();

			text_position.m_x = this->m_position.m_x - (this->m_size.m_x - frame->getwidth()) / 2;
			text_position.m_y = this->m_position.m_y - frame->getheight();

			this->m_animation_bullet_text.on_draw(camera, (int)text_position.m_x, (int)text_position.m_y);
		}
	}

	void on_attack()
	{
		Bullet* bullet = new Gamer2Bullet();

		Vector2 bullet_positon;
		const Vector2& bullet_size = bullet->getSize();
		bullet_positon.m_x = this->m_position.m_x + (this->m_size.m_x - bullet_size.m_x) / 2;
		bullet_positon.m_y = this->m_position.m_y;

		bullet->setPosition(bullet_positon.m_x, bullet_positon.m_y);
		bullet->setVelocity(is_facing_right ? this->velocity_bullet.m_x : -this->velocity_bullet.m_x, this->velocity_bullet.m_y);

		bullet->setCollideTarget(this->m_id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
		bullet->setCallback([&]() { this->m_mp += 35; });

		bullet_list.push_back(bullet);
	}

	void on_attack_ex()
	{
		this->is_attacking_ex = true;
		this->is_bullet_ex_visible = true;

		this->m_animation_bullet_text.reset();
		this->is_facing_right ? this->m_animation_attack_ex_right.reset() : this->m_animation_attack_ex_right.reset();

		Bullet* bullet = new Gamer2BulletEx();
		Player* target = (this->m_id == PlayerId::P1 ? player_2 : player_1);

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->getSize();
		const Vector2& target_size = target->getSize();
		const Vector2& target_position = target->getPosition();

		bullet_position.m_x = target_position.m_x + (target_size.m_x - bullet_size.m_x) / 2;
		bullet_position.m_y = -this->m_size.m_y;
		bullet_velocity.m_x = 0;
		bullet_velocity.m_y = this->speed_bullet_ex;

		bullet->setPosition(bullet_position.m_x, bullet_position.m_y);
		bullet->setVelocity(bullet_velocity.m_x, bullet_velocity.m_y);

		bullet->setCollideTarget(this->m_id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);

		bullet->setCallback([&]() {this->m_mp += 30; });

		bullet_list.push_back(bullet);
		mciSendString(L"play gamer2_bullet_text from 0", NULL, 0, NULL);
	}

private:
	const float speed_bullet_ex = 0.15f;
	const Vector2 velocity_bullet = { 0.25f, -0.5f };

	bool is_bullet_ex_visible = false;//设置是否现实特殊攻击子弹顶部文本

	Animation m_animation_bullet_text;

};

#endif // !_PLAYERGAMER2_H_