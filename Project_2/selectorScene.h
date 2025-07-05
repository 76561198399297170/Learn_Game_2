#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "utils.h"

#include "scene.h"
#include "sceneManager.h"

#include "animation.h"

#include "player.h"
#include "playerGamer1.h"
#include "playerGamer2.h"

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1P_desc;
extern IMAGE img_2P_desc;
extern IMAGE img_select_background_left;
extern IMAGE img_select_background_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_background;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_left;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_gamer1_selector_background_left;
extern IMAGE img_gamer1_selector_background_right;
extern IMAGE img_gamer2_selector_background_left;
extern IMAGE img_gamer2_selector_background_right;

extern Atlas atlas_gamer1_idle_right;
extern Atlas atlas_gamer2_idle_right;

extern IMAGE img_avatar_gamer1;
extern IMAGE img_avatar_gamer2;

extern SceneManager scene_manager;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	virtual void on_enter() 
	{
		this->m_animation_gamer1.setAtlas(&atlas_gamer1_idle_right);
		this->m_animation_gamer2.setAtlas(&atlas_gamer2_idle_right);
		this->m_animation_gamer1.setInterval(100);
		this->m_animation_gamer2.setInterval(100);

		static const int OFFSET_X = 50;

		this->pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		this->pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
		this->pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		this->pos_img_tip.y = getheight() - 125;
		this->pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		this->pos_img_1P.y = 35;
		this->pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		this->pos_img_2P.y = this->pos_img_1P.y;
		this->pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		this->pos_img_1P_desc.y = getheight() - 150;
		this->pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		this->pos_img_2P_desc.y = this->pos_img_1P_desc.y;
		this->pos_img_1P_select_background.x = (getwidth() / 2 - img_select_background_right.getwidth()) / 2 - OFFSET_X;
		this->pos_img_1P_select_background.y = this->pos_img_1P.y + img_1P.getwidth() + 35;
		this->pos_img_2P_select_background.x = getwidth() / 2 + (getwidth() / 2 - img_select_background_left.getwidth()) / 2 + OFFSET_X;
		this->pos_img_2P_select_background.y = this->pos_img_1P_select_background.y;
		this->pos_animation_1P.x = (getwidth() / 2 - atlas_gamer1_idle_right.getImage(0)->getwidth()) / 2 - OFFSET_X;
		this->pos_animation_1P.y = this->pos_img_1P_select_background.y + 80;
		this->pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_gamer1_idle_right.getImage(0)->getwidth()) / 2 + OFFSET_X;
		this->pos_animation_2P.y = this->pos_animation_1P.y;
		this->pos_img_1P_name.y = this->pos_animation_1P.y + 155;
		this->pos_img_2P_name.y = this->pos_img_1P_name.y;
		this->pos_1P_selector_btn_left.x = this->pos_img_1P_select_background.x - img_1P_selector_btn_idle_left.getwidth();
		this->pos_1P_selector_btn_left.y = this->pos_img_1P_select_background.y + (img_select_background_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
		this->pos_1P_selector_btn_right.x = this->pos_img_1P_select_background.x + img_select_background_right.getwidth();
		this->pos_1P_selector_btn_right.y = this->pos_1P_selector_btn_left.y;
		this->pos_2P_selector_btn_left.x = this->pos_img_2P_select_background.x - img_2P_selector_btn_idle_left.getwidth();
		this->pos_2P_selector_btn_left.y = this->pos_1P_selector_btn_left.y;
		this->pos_2P_selector_btn_right.x = this->pos_img_2P_select_background.x + img_select_background_left.getwidth();
		this->pos_2P_selector_btn_right.y = this->pos_1P_selector_btn_left.y;
	}

	virtual void on_update(int delta)
	{
		this->m_animation_gamer1.on_update(delta);
		this->m_animation_gamer2.on_update(delta);

		this->m_selector_background_scorll_offset_x += 5;
		if (this->m_selector_background_scorll_offset_x >= img_gamer1_selector_background_left.getwidth())
		{
			this->m_selector_background_scorll_offset_x -= img_gamer1_selector_background_left.getwidth();
		}
	}

	virtual void on_draw(const Camera& camera)
	{
		IMAGE* imgptr_P1_seletor_background = nullptr;
		IMAGE* imgptr_P2_seletor_background = nullptr;

		switch (this->m_player_type_1)
		{
		case PlayerType::Gamer1:
			imgptr_P1_seletor_background = &img_gamer1_selector_background_left;
			break;
		case PlayerType::Gamer2:
			imgptr_P1_seletor_background = &img_gamer2_selector_background_left;
			break;
		default:
			imgptr_P1_seletor_background = &img_gamer1_selector_background_left;
			break;
		}

		switch (this->m_player_type_2)
		{
		case PlayerType::Gamer1:
			imgptr_P2_seletor_background = &img_gamer1_selector_background_left;
			break;
		case PlayerType::Gamer2:
			imgptr_P2_seletor_background = &img_gamer2_selector_background_left;
			break;
		default:
			imgptr_P2_seletor_background = &img_gamer1_selector_background_left;
			break;
		}

		putimage(0, 0, &img_selector_background);

		putImageAlpha(this->m_selector_background_scorll_offset_x - imgptr_P1_seletor_background->getwidth(), 0, imgptr_P1_seletor_background);
		putImageAlpha(this->m_selector_background_scorll_offset_x, 0, imgptr_P1_seletor_background->getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P1_seletor_background, 0, 0);
		putImageAlpha(getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P2_seletor_background);
		putImageAlpha(getwidth() - imgptr_P2_seletor_background->getwidth(), 0, imgptr_P2_seletor_background->getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P2_seletor_background, this->m_selector_background_scorll_offset_x, 0);

		putImageAlpha(this->pos_img_VS.x, this->pos_img_VS.y, &img_VS);

		putImageAlpha(this->pos_img_1P.x, this->pos_img_1P.y, &img_1P);
		putImageAlpha(this->pos_img_2P.x, this->pos_img_2P.y, &img_2P);
		putImageAlpha(this->pos_img_1P_select_background.x, this->pos_img_1P_select_background.y, &img_select_background_right);
		putImageAlpha(this->pos_img_2P_select_background.x, this->pos_img_2P_select_background.y, &img_select_background_left);

		switch (this->m_player_type_1)
		{
		case PlayerType::Gamer1:
			this->m_animation_gamer1.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
			this->pos_img_1P_name.x = this->pos_img_1P_select_background.x + (img_select_background_right.getwidth() - textwidth(this->str_gamer1_name)) / 2;
			this->outtextxy_shaded(this->pos_img_1P_name.x, this->pos_img_1P_name.y, this->str_gamer1_name);
			break;
		case PlayerType::Gamer2:
			this->m_animation_gamer2.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
			this->pos_img_1P_name.x = this->pos_img_1P_select_background.x + (img_select_background_right.getwidth() - textwidth(this->str_gamer2_name)) / 2;
			this->outtextxy_shaded(this->pos_img_1P_name.x, this->pos_img_1P_name.y, this->str_gamer2_name);
			break;
		}
		
		switch (this->m_player_type_2)
		{
		case PlayerType::Gamer1:
			this->m_animation_gamer1.on_draw(camera, this->pos_animation_2P.x, this->pos_animation_2P.y);
			this->pos_img_2P_name.x = this->pos_img_2P_select_background.x + (img_select_background_left.getwidth() - textwidth(this->str_gamer1_name)) / 2;
			this->outtextxy_shaded(this->pos_img_2P_name.x, this->pos_img_2P_name.y, this->str_gamer1_name);
			break;
		case PlayerType::Gamer2:
			this->m_animation_gamer2.on_draw(camera, this->pos_animation_2P.x, this->pos_animation_2P.y);
			this->pos_img_2P_name.x = this->pos_img_2P_select_background.x + (img_select_background_left.getwidth() - textwidth(this->str_gamer2_name)) / 2;
			this->outtextxy_shaded(this->pos_img_2P_name.x, this->pos_img_2P_name.y, this->str_gamer2_name);
			break;
		}

		putImageAlpha(this->pos_1P_selector_btn_left.x, this->pos_1P_selector_btn_left.y, this->is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putImageAlpha(this->pos_1P_selector_btn_right.x, this->pos_1P_selector_btn_right.y, this->is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putImageAlpha(this->pos_2P_selector_btn_left.x, this->pos_2P_selector_btn_left.y, this->is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putImageAlpha(this->pos_2P_selector_btn_right.x, this->pos_2P_selector_btn_right.y, this->is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putImageAlpha(this->pos_img_1P_desc.x, this->pos_img_1P_desc.y, &img_1P_desc);
		putImageAlpha(this->pos_img_2P_desc.x, this->pos_img_2P_desc.y, &img_2P_desc);

		putImageAlpha(this->pos_img_tip.x, this->pos_img_tip.y, &img_selector_tip);
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case 0x41://'A'
				this->is_btn_1P_left_down = true;
				break;
			case 0x44://'D'
				this->is_btn_1P_right_down = true;
				break;
			case VK_LEFT://'¡û'
				this->is_btn_2P_left_down = true;
				break;
			case VK_RIGHT://'¡ú'
				this->is_btn_2P_right_down = true;
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x41://'A'
				this->is_btn_1P_left_down = false;
				this->m_player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)this->m_player_type_1 - 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case 0x44://'D'
				this->is_btn_1P_right_down = false;
				this->m_player_type_1 = (PlayerType)(((int)this->m_player_type_1 + 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_LEFT://'¡û'
				this->is_btn_2P_left_down = false;
				this->m_player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)this->m_player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_RIGHT://'¡ú'
				this->is_btn_2P_right_down = false;
				this->m_player_type_2 = (PlayerType)(((int)this->m_player_type_2 + 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_RETURN:
				scene_manager.switchTo(SceneManager::SceneType::Game);
				mciSendString(L"play ui_confirm from 0", NULL, 0, NULL);
				break;
			}
			break;
		default:
			break;
		}
	}

	virtual void on_exit()
	{
		switch (this->m_player_type_1)
		{
		case PlayerType::Gamer1:
			player_1 = new PlayerGamer1();
			img_player_1_avatar = &img_avatar_gamer1;
			break;
		case PlayerType::Gamer2:
			player_1 = new PlayerGamer2();
			img_player_1_avatar = &img_avatar_gamer2;
			break;
		}
		player_1->setId(PlayerId::P1);

		switch (this->m_player_type_2)
		{
		case PlayerType::Gamer1:
			player_2 = new PlayerGamer1(false);
			img_player_2_avatar = &img_avatar_gamer1;
			break;
		case PlayerType::Gamer2:
			player_2 = new PlayerGamer2(false);
			img_player_2_avatar = &img_avatar_gamer2;
			break;
		}
		player_2->setId(PlayerId::P2);

		mciSendString(L"stop bgm_menu", NULL, 0, NULL);
	}

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}

private:
	enum class PlayerType
	{
		Gamer1,
		Gamer2,
		Invalid
	};

private:
	bool is_btn_1P_left_down = false;
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;

private:
	POINT pos_img_VS = { 0 };
	POINT pos_img_tip = { 0 };
	POINT pos_img_1P = { 0 };
	POINT pos_img_2P = { 0 };
	POINT pos_img_1P_desc = { 0 };
	POINT pos_img_2P_desc = { 0 };
	POINT pos_img_1P_name = { 0 };
	POINT pos_img_2P_name = { 0 };
	POINT pos_animation_1P = { 0 };
	POINT pos_animation_2P = { 0 };
	POINT pos_img_1P_select_background = { 0 };
	POINT pos_img_2P_select_background = { 0 };
	POINT pos_1P_selector_btn_left = { 0 };
	POINT pos_2P_selector_btn_left = { 0 };
	POINT pos_1P_selector_btn_right = { 0 };
	POINT pos_2P_selector_btn_right = { 0 };

	Animation m_animation_gamer1;
	Animation m_animation_gamer2;

	PlayerType m_player_type_1 = PlayerType::Gamer1;
	PlayerType m_player_type_2 = PlayerType::Gamer2;

	LPCTSTR str_gamer1_name = L" ½ÇÉ« 1 ";
	LPCTSTR str_gamer2_name = L" ½ÇÉ« 2 ";

	int m_selector_background_scorll_offset_x = 0;

};

#endif