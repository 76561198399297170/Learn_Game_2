#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

extern SceneManager scene_manager;

extern IMAGE img_menu_background;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		mciSendString(L"play bgm_menu repeat from 0", NULL, 0, NULL);
	}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_menu_background);
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(L"play ui_confirm repeat from 0", NULL, 0, NULL);
			scene_manager.switchTo(SceneManager::SceneType::Selector);
		}
	}

	virtual void on_exit() {}

private:

};

#endif
