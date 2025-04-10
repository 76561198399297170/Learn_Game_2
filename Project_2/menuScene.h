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
		std::cout << "进入主菜单！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "主菜单运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"主菜单绘制");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "主菜单退出！" << std::endl;
	}

private:

};

#endif