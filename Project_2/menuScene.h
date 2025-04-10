#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"

#include <iostream>

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		std::cout << "�������˵���" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "���˵�������..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"���˵�����");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "���˵��˳���" << std::endl;
	}

private:

};

#endif