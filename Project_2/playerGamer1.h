#ifndef _PLAYERGAMER1_H_
#define _PLAYERGAMER1_H_

#include "player.h"

#include <iostream>

extern Atlas atlas_gamer1_idle_left;
extern Atlas atlas_gamer1_idle_right;
extern Atlas atlas_gamer1_run_left;
extern Atlas atlas_gamer1_run_right;

class PlayerGamer1 : public Player
{
public:
	PlayerGamer1()
	{
		this->m_animation_idle_left.setAtlas(&atlas_gamer1_idle_left);
		this->m_animation_idle_right.setAtlas(&atlas_gamer1_idle_right);
		this->m_animation_run_left.setAtlas(&atlas_gamer1_run_left);
		this->m_animation_run_right.setAtlas(&atlas_gamer1_run_right);

		this->m_animation_idle_left.setInterval(75);
		this->m_animation_idle_right.setInterval(75);
		this->m_animation_run_left.setInterval(75);
		this->m_animation_run_right.setInterval(75);
	}

	~PlayerGamer1() = default;

	virtual void on_update(int delta)
	{
		Player::on_update(delta);
		std::cout << "Gamer1 on update..." << std::endl;
	}

};

#endif // !_PLAYERGAMER1_H_
