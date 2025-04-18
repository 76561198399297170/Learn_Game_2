#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "utils.h"

#include "scene.h"
#include "sceneManager.h"

#include "animation.h"

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
		this->pos_2P_selector_btn_left.x = this->pos_img_2P_select_background.x + img_2P_selector_btn_idle_left.getwidth();
		this->pos_2P_selector_btn_left.y = this->pos_1P_selector_btn_left.y;
		this->pos_2P_selector_btn_right.x = this->pos_img_2P_select_background.x + img_select_background_left.getwidth();
		this->pos_2P_selector_btn_right.y = this->pos_1P_selector_btn_left.y;
	}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_selector_background);
		putImageAlpha(this->pos_img_VS.x, this->pos_img_VS.y, &img_VS);

		putImageAlpha(this->pos_img_1P.x, this->pos_img_1P.y, &img_1P);
		putImageAlpha(this->pos_img_2P.x, this->pos_img_2P.y, &img_2P);
		putImageAlpha(this->pos_img_1P_select_background.x, this->pos_img_1P_select_background.y, &img_select_background_right);
		putImageAlpha(this->pos_img_2P_select_background.x, this->pos_img_2P_select_background.y, &img_select_background_left);

		putImageAlpha(this->pos_img_1P_desc.x, this->pos_img_1P_desc.y, &img_1P_desc);
		putImageAlpha(this->pos_img_2P_desc.x, this->pos_img_2P_desc.y, &img_2P_desc);

		putImageAlpha(this->pos_img_tip.x, this->pos_img_tip.y, &img_selector_tip);
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit() {}

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

};

#endif