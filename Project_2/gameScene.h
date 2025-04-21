#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "utils.h"

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		this->pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		this->pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		this->pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		this->pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
	}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera) 
	{
		putImageAlpha(camera, this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
		putImageAlpha(camera, this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit() {}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

};

#endif