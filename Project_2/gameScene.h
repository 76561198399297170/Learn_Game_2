#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

#include <iostream>

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		std::cout << "������Ϸ���ڳ�����" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "��Ϸ������..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"��Ϸ���ڻ���");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "��Ϸ�����˳���" << std::endl;
	}

private:

};

#endif