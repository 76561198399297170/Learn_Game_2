#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "utils.h"
#include "platform.h"
#include "scene.h"
#include "sceneManager.h"
#include "player.h"
#include "statusBar.h"

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern IMAGE img_winner_bar;
extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern Camera main_camera;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		this->is_game_over = false;
		this->is_slide_out_started = false;

		this->m_pos_img_winner_bar.x = -img_winner_bar.getwidth();
		this->m_pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		this->m_pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		this->m_pos_img_winner_text.x = this->m_pos_img_winner_bar.x;
		this->m_pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		this->m_pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		this->m_timer_winner_slide_in.restart();
		this->m_timer_winner_slide_in.setWaitTime(2500);
		this->m_timer_winner_slide_in.setOneShot(true);
		this->m_timer_winner_slide_in.setCallback([&]() {this->is_slide_out_started = true; });

		this->m_timer_winner_slide_out.restart();
		this->m_timer_winner_slide_out.setWaitTime(1000);
		this->m_timer_winner_slide_out.setOneShot(true);
		this->m_timer_winner_slide_out.setCallback([&]() {scene_manager.switchTo(SceneManager::SceneType::Menu); });

		this->m_status_bar_1P.setAvatar(img_player_1_avatar);
		this->m_status_bar_2P.setAvatar(img_player_2_avatar);

		this->m_status_bar_1P.setPosition(235, 625);
		this->m_status_bar_2P.setPosition(675, 625);

		player_1->setPosition(200, 50);
		player_2->setPosition(975, 50);

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

		mciSendString(L"play bgm_game repeat from 0", NULL, 0, NULL);
	}

	virtual void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_updata(delta);

		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), 
			[](const Bullet* b)
			{
				bool deletable = b->checkCanRemove();
				if (deletable) delete b;
				return deletable;
			}), 
			bullet_list.end());

		for (Bullet* b : bullet_list) b->on_update(delta);

		const Vector2& position_player_1 = player_1->getPosition();
		const Vector2& position_player_2 = player_2->getPosition();
		if (position_player_1.m_y >= getheight()) player_1->setHp(0);
		if (position_player_2.m_y >= getheight()) player_2->setHp(0);

		if (player_1->getHp() <= 0 || player_2->getHp() <= 0)
		{
			if (!this->is_game_over)
			{
				mciSendString(L"stop bgm_game", NULL, 0, NULL);
				mciSendString(L"play ui_win from 0", NULL, 0, NULL);
			}

			this->is_game_over = true;
		}

		this->m_status_bar_1P.setHp(player_1->getHp());
		this->m_status_bar_1P.setMp(player_1->getMp());
		this->m_status_bar_2P.setHp(player_2->getHp());
		this->m_status_bar_2P.setMp(player_2->getMp());

		if (this->is_game_over)
		{
			this->m_pos_img_winner_bar.x += (int)(this->speed_winner_bar * delta);
			this->m_pos_img_winner_text.x += (int)(this->speed_winner_text * delta);

			if (!this->is_slide_out_started)
			{
				this->m_timer_winner_slide_in.on_updata(delta);
				if (this->m_pos_img_winner_bar.x > this->m_pos_x_img_winner_bar_dst) this->m_pos_img_winner_bar.x = this->m_pos_x_img_winner_bar_dst;
				if (this->m_pos_img_winner_text.x > this->m_pos_x_img_winner_text_dst) this->m_pos_img_winner_text.x = this->m_pos_x_img_winner_text_dst;
			}
			else
			{
				this->m_timer_winner_slide_out.on_updata(delta);
			}
		}
	}

	virtual void on_draw(const Camera& camera) 
	{
		putImageAlpha(camera, this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
		putImageAlpha(camera, this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);

		for (const Platform& p : platform_list) p.on_draw(camera);

		player_1->on_draw(camera);
		player_2->on_draw(camera);

		for (const Bullet* b : bullet_list) b->on_draw(camera);
		
		if (this->is_game_over)
		{
			putImageAlpha(this->m_pos_img_winner_bar.x, this->m_pos_img_winner_bar.y, &img_winner_bar);
			putImageAlpha(this->m_pos_img_winner_text.x, this->m_pos_img_winner_text.y,
				player_1->getHp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else
		{
			this->m_status_bar_1P.on_draw();
			this->m_status_bar_2P.on_draw();
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		player_2->on_input(msg);

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

	virtual void on_exit()
	{
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
	}

private:
	const float speed_winner_bar = 3.0f;
	const float speed_winner_text = 1.5f;

	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

	StatusBar m_status_bar_1P;
	StatusBar m_status_bar_2P;

	bool is_game_over = false;

	bool is_slide_out_started = false;

	POINT m_pos_img_winner_bar = { 0 };
	POINT m_pos_img_winner_text = { 0 };

	int m_pos_x_img_winner_bar_dst = 0;
	int m_pos_x_img_winner_text_dst = 0;

	Timer m_timer_winner_slide_in;
	Timer m_timer_winner_slide_out;


};

#endif