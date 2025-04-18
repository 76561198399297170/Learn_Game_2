#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include "timer.h"
#include "animation.h"
#include "camera.h"

#include <iostream>

extern Atlas atlas_gamer1_run_right;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		this->m_animation_gamer1_run_right.setAtlas(&atlas_gamer1_run_right);
		this->m_animation_gamer1_run_right.setInterval(75);
		this->m_animation_gamer1_run_right.setLoop(true);
		
		this->m_timer.setWaitTime(1000);
		this->m_timer.setOneShot(false);
		this->m_timer.setCallback(
			[]()
			{
				std::cout << "WOOF!!" << std::endl;
			}
		);
	}

	virtual void on_update(int delta)
	{
		this->m_timer.on_updata(delta);
		this->m_camera.on_updata(delta);
		this->m_animation_gamer1_run_right.on_update(delta);
	}

	virtual void on_draw()
	{
		const Vector2& pos_camera = this->m_camera.getPosition();
		this->m_animation_gamer1_run_right.on_draw((int)(100 - pos_camera.m_x), (int)(100 - pos_camera.m_y));
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_SPACE) this->m_camera.shake(10, 350);
			else scene_manager.switchTo(SceneManager::SceneType::Selector);
		}
	}

	virtual void on_exit()
	{
		std::cout << "Ö÷²Ëµ¥ÍË³ö£¡" << std::endl;
	}

private:
	Animation m_animation_gamer1_run_right;
	Camera m_camera;
	Timer m_timer;

};

#endif