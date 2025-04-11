#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

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

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Selector);
		}
	}

	virtual void on_exit()
	{
		std::cout << "���˵��˳���" << std::endl;
	}

private:

};

#endif