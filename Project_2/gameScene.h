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

	virtual void on_enter() {}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera) {}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit() {}

private:

};

#endif