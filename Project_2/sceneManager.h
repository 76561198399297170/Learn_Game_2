#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void setCurrentState(Scene* scene)
	{
		this->m_current_scene = scene;
		this->m_current_scene->on_enter();
	}

public:
	Scene* m_current_scene = nullptr;

};

#endif