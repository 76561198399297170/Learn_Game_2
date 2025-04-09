#include "scene.h"
#include "menuScene.h"

#include <graphics.h>

const int FPS = 60;

int main()
{
	ExMessage msg;

	initgraph(1200, 720);

	Scene* scene = new MenuScene();
	scene->on_enter();

	BeginBatchDraw();
	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene->on_input(msg);
		}
		scene->on_update();

		cleardevice();
		scene->on_draw();

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