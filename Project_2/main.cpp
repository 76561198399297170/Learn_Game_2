#include "scene.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"
#include "sceneManager.h"

#include <graphics.h>

const int FPS = 60;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;

int main()
{
	ExMessage msg;

	initgraph(1200, 720);

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();
	scene_manager.setCurrentState(menu_scene);

	BeginBatchDraw();
	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}
		scene_manager.on_updata();

		cleardevice();
		scene_manager.on_draw();

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