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
		std::cout << "������Ϸ���ڳ�����" << std::endl;
	}

	virtual void on_update(int delta)
	{
		std::cout << "��Ϸ������..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"��Ϸ���ڻ���");
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
		std::cout << "��Ϸ�����˳���" << std::endl;
	}

private:

};

#endif