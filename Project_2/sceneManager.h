#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void setCurrentState(Scene* scene)
	{
		this->m_current_scene = scene;
		this->m_current_scene->on_enter();
	}

	void switchTo(SceneType type)
	{
		this->m_current_scene->on_exit();
		switch (type)
		{
		case SceneManager::SceneType::Menu:
			this->m_current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			this->m_current_scene = game_scene;
			break;
		case SceneManager::SceneType::Selector:
			this->m_current_scene = selector_scene;
			break;
		default:
			break;
		}
		this->m_current_scene->on_enter();
	}

	void on_updata(int delta)
	{
		this->m_current_scene->on_update(delta);
	}

	void on_draw()
	{
		this->m_current_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		this->m_current_scene->on_input(msg);
	}

public:
	Scene* m_current_scene = nullptr;

};

#endif