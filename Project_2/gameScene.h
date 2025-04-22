#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "utils.h"
#include "platform.h"
#include "scene.h"
#include "sceneManager.h"
#include "player.h"

#include <iostream>

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;

extern std::vector<Platform> platform_list;

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

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform.m_img = &img_platform_large;
		large_platform.m_render_position.x = 122;
		large_platform.m_render_position.y = 455;
		large_platform.m_shape.left = (float)large_platform.m_render_position.x + 30;
		large_platform.m_shape.right = (float)large_platform.m_render_position.x + img_platform_large.getwidth() - 30;
		large_platform.m_shape.y = (float)large_platform.m_render_position.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.m_img = &img_platform_small;
		small_platform_1.m_render_position.x = 175;
		small_platform_1.m_render_position.y = 360;
		small_platform_1.m_shape.left = (float)small_platform_1.m_render_position.x + 40;
		small_platform_1.m_shape.right = (float)small_platform_1.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_1.m_shape.y = (float)small_platform_1.m_render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.m_img = &img_platform_small;
		small_platform_2.m_render_position.x = 855;
		small_platform_2.m_render_position.y = 360;
		small_platform_2.m_shape.left = (float)small_platform_2.m_render_position.x + 40;
		small_platform_2.m_shape.right = (float)small_platform_2.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_2.m_shape.y = (float)small_platform_2.m_render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.m_img = &img_platform_small;
		small_platform_3.m_render_position.x = 515;
		small_platform_3.m_render_position.y = 255;
		small_platform_3.m_shape.left = (float)small_platform_3.m_render_position.x + 40;
		small_platform_3.m_shape.right = (float)small_platform_3.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_3.m_shape.y = (float)small_platform_3.m_render_position.y + img_platform_small.getheight() / 2;

	}

	virtual void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);
	}

	virtual void on_draw(const Camera& camera) 
	{
		putImageAlpha(camera, this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
		putImageAlpha(camera, this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);

		for (const Platform& p : platform_list)
		{
			p.on_draw(camera);
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x51://'Q'
				is_debug = !is_debug;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	virtual void on_exit() {}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

};

#endif