#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入游戏局内场景！" << std::endl;
	}

	virtual void on_update(int delta)
	{
		std::cout << "游戏运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"游戏局内绘制");
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Menu);
		}
	}

	virtual void on_exit()
	{
		std::cout << "游戏局内退出！" << std::endl;
	}

private:

};

#endif