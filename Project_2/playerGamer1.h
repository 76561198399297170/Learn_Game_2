#ifndef _PLAYERGAMER1_H_
#define _PLAYERGAMER1_H_

#include "player.h"

#include <iostream>

class PlayerGamer1 : public Player
{
public:
	PlayerGamer1() = default;
	~PlayerGamer1() = default;

	virtual void on_update(int delta)
	{
		std::cout << "Gamer1 on update..." << std::endl;
	}

};

#endif // !_PLAYERGAMER1_H_
