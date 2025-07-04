#include "atlas.h"
#include "camera.h"

#include "scene.h"
#include "sceneManager.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"

#include "utils.h"
#include "platform.h"
#include "bullet.h"

#include "player.h"
#include "playerGamer1.h"
#include "playerGamer2.h"

#pragma comment(lib, "Winmm.lib")

#include <graphics.h>
#include <vector>

const int FPS = 60;

bool is_debug = false;

Camera main_camera;

std::vector<Platform> platform_list;
std::vector<Bullet*> bullet_list;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

IMAGE img_menu_background;					//主菜单背景图片

IMAGE img_VS;								//VS 艺术字图片
IMAGE img_1P;								//1P 文本图片
IMAGE img_2P;								//2P 文本图片
IMAGE img_1P_desc;							//1P 键位描述图片
IMAGE img_2P_desc;							//2P 键位描述图片
IMAGE img_select_background_left;			//选人朝左背景图片
IMAGE img_select_background_right;			//选人朝右背景图片
IMAGE img_selector_tip;						//选人界面提示信息
IMAGE img_selector_background;				//选人界面背景图
IMAGE img_1P_selector_btn_idle_left;		//1P 向左选择按钮默认状态图片
IMAGE img_1P_selector_btn_idle_right;		//1P 向右选择按钮默认状态图片
IMAGE img_1P_selector_btn_down_left;		//1P 向左选择按钮按下状态图片
IMAGE img_1P_selector_btn_down_right;		//1P 向右选择按钮按下状态图片
IMAGE img_2P_selector_btn_idle_left;		//2P 向左选择按钮默认状态图片
IMAGE img_2P_selector_btn_idle_right;		//2P 向右选择按钮默认状态图片
IMAGE img_2P_selector_btn_down_left;		//2P 向左选择按钮按下状态图片
IMAGE img_2P_selector_btn_down_right;		//2P 向右选择按钮按下状态图片
IMAGE img_gamer1_selector_background_left;	//选人界面类型1朝左背景图片
IMAGE img_gamer1_selector_background_right;	//选人界面类型1朝右背景图片
IMAGE img_gamer2_selector_background_left;	//选人界面类型2朝左背景图片
IMAGE img_gamer2_selector_background_right;	//选人界面类型2朝右背景图片

IMAGE img_sky;								//填空图片
IMAGE img_hills;							//山脉图片
IMAGE img_platform_large;					//大型平台图片
IMAGE img_platform_small;					//小型平台图片

IMAGE img_1P_cursor;						//1P 指示光标图片
IMAGE img_2P_cursor;						//2P 指示光标图片

Atlas atlas_gamer1_idle_left;				//类型1向左默认动画图集
Atlas atlas_gamer1_idle_right;				//类型1向右默认动画图集
Atlas atlas_gamer1_run_left;				//类型1向左奔跑动画图集
Atlas atlas_gamer1_run_right;				//类型1向右奔跑动画图集
Atlas atlas_gamer1_attack_ex_left;			//类型1向左特殊攻击动画图集
Atlas atlas_gamer1_attack_ex_right;			//类型1向右特殊攻击动画图集
Atlas atlas_gamer1_die_left;				//类型1向左死亡动画图集
Atlas atlas_gamer1_die_right;				//类型1向右死亡动画图集

Atlas atlas_gamer2_idle_left;				//类型2向左默认动画图集
Atlas atlas_gamer2_idle_right;				//类型2向右默认动画图集
Atlas atlas_gamer2_run_left;				//类型2向左奔跑动画图集
Atlas atlas_gamer2_run_right;				//类型2向右奔跑动画图集
Atlas atlas_gamer2_attack_ex_left;			//类型2向左特殊攻击动画图集
Atlas atlas_gamer2_attack_ex_right;			//类型2向右特殊攻击动画图集
Atlas atlas_gamer2_die_left;				//类型2向左死亡动画图集
Atlas atlas_gamer2_die_right;				//类型2向右死亡动画图集

IMAGE img_gamer1_bullet;					//类型1子弹图片
Atlas atlas_gamer1_bullet_break;			//类型1子弹破碎动画图集
Atlas atlas_gemer2_bullet;					//类型2子弹动画图集
Atlas atlas_gemer2_bullet_explode;			//类型2子弹爆炸动画图集
Atlas atlas_gamer2_bullet_ex;				//类型2特殊类型子弹动画图集
Atlas atlas_gamer2_bullet_ex_explode;		//类型2特殊类型子弹爆炸动画图集
Atlas atlas_gamer2_bullet_text;				//类型2特殊类型子弹爆炸文本动画图集

Atlas atlas_run_effect;						//奔跑特效动画图集
Atlas atlas_jump_effect;					//跳跃特效动画图集
Atlas atlas_land_effect;					//落地特效动画图集

IMAGE img_winner_bar;						//获胜玩家背景图片
IMAGE img_1P_winner;						//1P 获胜文本图片
IMAGE img_2P_winner;						//2P 获胜文本图片

IMAGE img_avatar_gamer1;					//类型1头像图片
IMAGE img_avatar_gamer2;					//类型2头像图片

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;

void flipAtlas(Atlas& src, Atlas& dst)
{
	dst.clear();
	for (int i = 0; i < src.getSize(); i++)
	{
		IMAGE img_flpipped;
		flipImage(src.getImage(i), &img_flpipped);
		dst.addImage(img_flpipped);
	}
}

void loadGameResources()
{
	AddFontResourceEx(L"./resources/HYPixel11pxU-2.ttf", FR_PRIVATE, NULL);

	loadimage(&img_menu_background, L"./resources/menu_background.png");

	loadimage(&img_VS, L"./resources/VS.png");
	loadimage(&img_1P, L"./resources/1P.png");
	loadimage(&img_2P, L"./resources/2P.png");
	loadimage(&img_1P_desc, L"./resources/1P_desc.png");
	loadimage(&img_2P_desc, L"./resources/2P_desc.png");
	loadimage(&img_select_background_right, L"./resources/select_background.png");
	flipImage(&img_select_background_right, &img_select_background_left);
	loadimage(&img_selector_tip, L"./resources/selector_tip.png");
	loadimage(&img_selector_background, L"./resources/selector_background.png");
	loadimage(&img_1P_selector_btn_idle_right, L"./resources/1P_selector_btn_idle.png");
	flipImage(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
	loadimage(&img_1P_selector_btn_down_right, L"./resources/1P_selector_btn_down.png");
	flipImage(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
	loadimage(&img_2P_selector_btn_idle_right, L"./resources/2P_selector_btn_idle.png");
	flipImage(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
	loadimage(&img_2P_selector_btn_down_right, L"./resources/2P_selector_btn_down.png");
	flipImage(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
	loadimage(&img_gamer1_selector_background_right, L"./resources/gamer1_selector_background.png");
	flipImage(&img_gamer1_selector_background_right, &img_gamer1_selector_background_left);
	loadimage(&img_gamer2_selector_background_right, L"./resources/gamer2_selector_background.png");
	flipImage(&img_gamer2_selector_background_right, &img_gamer2_selector_background_left);

	loadimage(&img_sky, L"./resources/sky.png");
	loadimage(&img_hills, L"./resources/hills.png");
	loadimage(&img_platform_large, L"./resources/platform_large.png");
	loadimage(&img_platform_small, L"./resources/platform_small.png");

	loadimage(&img_1P_cursor, L"./resources/1P_cursor.png");
	loadimage(&img_2P_cursor, L"./resources/2P_cursor.png");

	atlas_gamer1_idle_right.loadFromFile(L"./resources/gamer1_idle_%d.png", 9);
	flipAtlas(atlas_gamer1_idle_right, atlas_gamer1_idle_left);
	atlas_gamer1_run_right.loadFromFile(L"./resources/gamer1_run_%d.png", 5);
	flipAtlas(atlas_gamer1_run_right, atlas_gamer1_run_left);
	atlas_gamer1_attack_ex_right.loadFromFile(L"./resources/gamer1_attack_ex_%d.png", 3);
	flipAtlas(atlas_gamer1_attack_ex_right, atlas_gamer1_attack_ex_left);
	atlas_gamer1_die_right.loadFromFile(L"./resources/gamer1_die_%d.png", 4);
	flipAtlas(atlas_gamer1_die_right, atlas_gamer1_die_left);

	atlas_gamer2_idle_right.loadFromFile(L"./resources/gamer2_idle_%d.png", 8);
	flipAtlas(atlas_gamer2_idle_right, atlas_gamer2_idle_left);
	atlas_gamer2_run_right.loadFromFile(L"./resources/gamer2_run_%d.png", 5);
	flipAtlas(atlas_gamer2_run_right, atlas_gamer2_run_left);
	atlas_gamer2_attack_ex_right.loadFromFile(L"./resources/gamer2_attack_ex_%d.png", 9);
	flipAtlas(atlas_gamer2_attack_ex_right, atlas_gamer2_attack_ex_left);
	atlas_gamer2_die_right.loadFromFile(L"./resources/gamer2_die_%d.png", 2);
	flipAtlas(atlas_gamer2_die_right, atlas_gamer2_die_left);

	loadimage(&img_gamer1_bullet, L"./resources/gamer1_bullet.png");
	atlas_gamer1_bullet_break.loadFromFile(L"./resources/gamer1_bullet_break_%d.png", 3);
	atlas_gemer2_bullet.loadFromFile(L"./resources/gamer2_bullet_%d.png", 5);
	atlas_gemer2_bullet_explode.loadFromFile(L"./resources/gamer2_bullet_explode_%d.png", 5);
	atlas_gamer2_bullet_ex.loadFromFile(L"./resources/gamer2_bullet_ex_%d.png", 5);
	atlas_gamer2_bullet_ex_explode.loadFromFile(L"./resources/gamer2_bullet_ex_explode_%d.png", 5);
	atlas_gamer2_bullet_text.loadFromFile(L"./resources/gamer2_bullet_text_%d.png", 6);

	atlas_run_effect.loadFromFile(L"./resources/run_effect_%d.png", 4);
	atlas_jump_effect.loadFromFile(L"./resources/jump_effect_%d.png", 5);
	atlas_land_effect.loadFromFile(L"./resources/land_effect_%d.png", 2);

	loadimage(&img_1P_winner, L"./resources/1P_winner.png");
	loadimage(&img_2P_winner, L"./resources/2P_winner.png");
	loadimage(&img_winner_bar, L"./resources/winner_bar.png");

	loadimage(&img_avatar_gamer1, L"./resources/avatar_gamer1.png");
	loadimage(&img_avatar_gamer2, L"./resources/avatar_gamer2.png");

	mciSendString(L"open ./resources/bgm_game.mp3 alias bgm_game", NULL, 0, NULL);
	mciSendString(L"open ./resources/bgm_menu.mp3 alias bgm_menu", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_break_1.mp3 alias gamer1_bullet_break_1", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_break_2.mp3 alias gamer1_bullet_break_2", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_break_3.mp3 alias gamer1_bullet_break_3", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_shoot_1.mp3 alias gamer1_bullet_shoot_1", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_shoot_2.mp3 alias gamer1_bullet_shoot_2", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer1_bullet_shoot_ex.mp3 alias gamer1_bullet_shoot_ex", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer2_bullet_explode.mp3 alias gamer2_bullet_explode", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer2_bullet_explode_ex.mp3 alias gamer2_bullet_explode_ex", NULL, 0, NULL);
	mciSendString(L"open ./resources/gamer2_bullet_text.mp3 alias gamer2_bullet_text", NULL, 0, NULL);
	mciSendString(L"open ./resources/ui_confirm.wav alias ui_confirm", NULL, 0, NULL);
	mciSendString(L"open ./resources/ui_switch.wav alias ui_switch", NULL, 0, NULL);
	mciSendString(L"open ./resources/ui_win.wav alias ui_win", NULL, 0, NULL);

	return;
}

int main()
{
	ExMessage msg;

	loadGameResources();
	
	initgraph(1200, 720);

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.setCurrentState(menu_scene);

	settextstyle(28, 0, L"HYPixel11pxU-2.ttf");
	setbkmode(TRANSPARENT);

	BeginBatchDraw();
	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		scene_manager.on_updata(delta_tick_time);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(main_camera);

		FlushBatchDraw();

		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - frame_delta_time);
		}

	}
	EndBatchDraw();

	return 0;
}