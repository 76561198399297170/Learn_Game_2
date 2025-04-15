#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	virtual void on_enter()
	{
		std::cout << "����ѡ�ˣ�" << std::endl;
	}

	virtual void on_update(int delta)
	{
		std::cout << "ѡ��������..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"ѡ�˻���");
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Game);
		}
	}

	virtual void on_exit()
	{
		std::cout << "ѡ���˳���" << std::endl;
	}

private:

};

#endif