#ifndef _PLAYERGAMER2_H_
#define _PLAYERGAMER2_H_

#include "player.h"

#include <iostream>

extern Atlas atlas_gamer2_idle_left;
extern Atlas atlas_gamer2_idle_right;
extern Atlas atlas_gamer2_run_left;
extern Atlas atlas_gamer2_run_right;

class PlayerGamer2 : public Player
{
public:
	PlayerGamer2()
	{
		this->m_animation_idle_left.setAtlas(&atlas_gamer2_idle_left);
		this->m_animation_idle_right.setAtlas(&atlas_gamer2_idle_right);
		this->m_animation_run_left.setAtlas(&atlas_gamer2_run_left);
		this->m_animation_run_right.setAtlas(&atlas_gamer2_run_right);

		this->m_animation_idle_left.setInterval(75);
		this->m_animation_idle_right.setInterval(75);
		this->m_animation_run_left.setInterval(75);
		this->m_animation_run_right.setInterval(75);

		this->m_size.m_x = 96;
		this->m_size.m_y = 96;
	}

	~PlayerGamer2() = default;

	virtual void on_update(int delta)
	{
		Player::on_update(delta);
		std::cout << "Gamer2 on update..." << std::endl;
	}

};

#endif // !_PLAYERGAMER2_H_