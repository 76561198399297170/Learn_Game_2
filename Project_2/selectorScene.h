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
		std::cout << "进入选人！" << std::endl;
	}

	virtual void on_update(int delta)
	{
		std::cout << "选人运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"选人绘制");
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
		std::cout << "选人退出！" << std::endl;
	}

private:

};

#endif