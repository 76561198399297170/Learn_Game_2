#ifndef _PLAYERGAMER2_H_
#define _PLAYERGAMER2_H_

#include "player.h"

#include <iostream>

class PlayerGamer2 : public Player
{
public:
	PlayerGamer2() = default;
	~PlayerGamer2() = default;

	virtual void on_update(int delta)
	{
		std::cout << "Gamer2 on update..." << std::endl;
	}

};

#endif // !_PLAYERGAMER2_H_