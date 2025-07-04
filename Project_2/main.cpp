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

IMAGE img_menu_background;					//���˵�����ͼƬ

IMAGE img_VS;								//VS ������ͼƬ
IMAGE img_1P;								//1P �ı�ͼƬ
IMAGE img_2P;								//2P �ı�ͼƬ
IMAGE img_1P_desc;							//1P ��λ����ͼƬ
IMAGE img_2P_desc;							//2P ��λ����ͼƬ
IMAGE img_select_background_left;			//ѡ�˳��󱳾�ͼƬ
IMAGE img_select_background_right;			//ѡ�˳��ұ���ͼƬ
IMAGE img_selector_tip;						//ѡ�˽�����ʾ��Ϣ
IMAGE img_selector_background;				//ѡ�˽��汳��ͼ
IMAGE img_1P_selector_btn_idle_left;		//1P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_1P_selector_btn_idle_right;		//1P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_1P_selector_btn_down_left;		//1P ����ѡ��ť����״̬ͼƬ
IMAGE img_1P_selector_btn_down_right;		//1P ����ѡ��ť����״̬ͼƬ
IMAGE img_2P_selector_btn_idle_left;		//2P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_2P_selector_btn_idle_right;		//2P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_2P_selector_btn_down_left;		//2P ����ѡ��ť����״̬ͼƬ
IMAGE img_2P_selector_btn_down_right;		//2P ����ѡ��ť����״̬ͼƬ
IMAGE img_gamer1_selector_background_left;	//ѡ�˽�������1���󱳾�ͼƬ
IMAGE img_gamer1_selector_background_right;	//ѡ�˽�������1���ұ���ͼƬ
IMAGE img_gamer2_selector_background_left;	//ѡ�˽�������2���󱳾�ͼƬ
IMAGE img_gamer2_selector_background_right;	//ѡ�˽�������2���ұ���ͼƬ

IMAGE img_sky;								//���ͼƬ
IMAGE img_hills;							//ɽ��ͼƬ
IMAGE img_platform_large;					//����ƽ̨ͼƬ
IMAGE img_platform_small;					//С��ƽ̨ͼƬ

IMAGE img_1P_cursor;						//1P ָʾ���ͼƬ
IMAGE img_2P_cursor;						//2P ָʾ���ͼƬ

Atlas atlas_gamer1_idle_left;				//����1����Ĭ�϶���ͼ��
Atlas atlas_gamer1_idle_right;				//����1����Ĭ�϶���ͼ��
Atlas atlas_gamer1_run_left;				//����1�����ܶ���ͼ��
Atlas atlas_gamer1_run_right;				//����1���ұ��ܶ���ͼ��
Atlas atlas_gamer1_attack_ex_left;			//����1�������⹥������ͼ��
Atlas atlas_gamer1_attack_ex_right;			//����1�������⹥������ͼ��
Atlas atlas_gamer1_die_left;				//����1������������ͼ��
Atlas atlas_gamer1_die_right;				//����1������������ͼ��

Atlas atlas_gamer2_idle_left;				//����2����Ĭ�϶���ͼ��
Atlas atlas_gamer2_idle_right;				//����2����Ĭ�϶���ͼ��
Atlas atlas_gamer2_run_left;				//����2�����ܶ���ͼ��
Atlas atlas_gamer2_run_right;				//����2���ұ��ܶ���ͼ��
Atlas atlas_gamer2_attack_ex_left;			//����2�������⹥������ͼ��
Atlas atlas_gamer2_attack_ex_right;			//����2�������⹥������ͼ��
Atlas atlas_gamer2_die_left;				//����2������������ͼ��
Atlas atlas_gamer2_die_right;				//����2������������ͼ��

IMAGE img_gamer1_bullet;					//����1�ӵ�ͼƬ
Atlas atlas_gamer1_bullet_break;			//����1�ӵ����鶯��ͼ��
Atlas atlas_gemer2_bullet;					//����2�ӵ�����ͼ��
Atlas atlas_gemer2_bullet_explode;			//����2�ӵ���ը����ͼ��
Atlas atlas_gamer2_bullet_ex;				//����2���������ӵ�����ͼ��
Atlas atlas_gamer2_bullet_ex_explode;		//����2���������ӵ���ը����ͼ��
Atlas atlas_gamer2_bullet_text;				//����2���������ӵ���ը�ı�����ͼ��

Atlas atlas_run_effect;						//������Ч����ͼ��
Atlas atlas_jump_effect;					//��Ծ��Ч����ͼ��
Atlas atlas_land_effect;					//�����Ч����ͼ��

IMAGE img_winner_bar;						//��ʤ��ұ���ͼƬ
IMAGE img_1P_winner;						//1P ��ʤ�ı�ͼƬ
IMAGE img_2P_winner;						//2P ��ʤ�ı�ͼƬ

IMAGE img_avatar_gamer1;					//����1ͷ��ͼƬ
IMAGE img_avatar_gamer2;					//����2ͷ��ͼƬ

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