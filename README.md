# 学习记录

## 第一节

### 回顾与引言

在前面的项目中，无论是游戏局内逻辑还是主菜单逻辑，以及动画封装各种累都放在了main.cpp下，全部代码写在一个文件下就会显得文件很臃肿，不仅会导致命名空间和依赖关系混乱，在调试修改时也不容易定位代码位置。所以首先我们需要将项目不同阶段代码放置到不同类中封装，并将不同的类封装到不同头文件与源文件中。

在割草游戏项目的最后，我们可以优化场景类型定义Stage枚举类区分不同游戏阶段，随后根据不同阶段进行不同的事件处理和绘图逻辑（有点像状态机呢），这就类似最原始的场景管理思想了。

什么是场景？假设程序项目是一场演出，演出过程中又又不同的幕，在不同幕中又有不同“剧本”的逻辑和“角色”，其中的“角色”就是游戏开发中经常提到的GameObject，无论是玩家敌人还是子弹从概念上都是GameObject的范畴，接收不同场景“剧情”的指挥，以上便可以对项目场景从宏观上进行简单划分了：

首先是主菜单为一个场景，随后选择界面又是一个场景，最后游戏局内逻辑也需要在一个单独场景中进行。

所以我们可以定义场景基类Scene，而以上三个场景继承这个基类场景，通过多态实现不同场景逻辑，回顾之前的内容，项目的主体循环使用while先进行读取操作，处理数据，绘制画面最后动态延时的方法，在结束循环后释放资源，也就是：

```
#include <graphics.h>

const int FPS = 60;

int main()
{
	ExMessage msg;

	initgraph(1200, 720, EW_SHOWCONSOLE);

	BeginBatchDraw();
	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			//读取操作
		}
		//更新数据

		cleardevice();
		//绘制图像

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
```



### 文件包含与基类场景接口

为了完成上面Scene场景类的封装和实践，我们创建scene.h文件，此时生成的文件仅有一行#pragma once表示防止重复包含头文件：

include操作即纯粹的将包含文件代码复制粘贴到include的位置，而比如a.h和b.h都包含了c.h，最后main.cpp包含a.h和b.h就会导致c.h重复包含两次报错，为了防止所以使用这一行代码防止重复包含。

除了#pragma once还可以使用if not define指令进行包含，如果未定义宏就进行定义并包含代码文件，反之不进行任何操作：

```
#ifndef _SCENE_H_
#define _SCENE_H_

#endif
```



完成基类Scene的创建与部分逻辑如下，注意多态要使用virtual虚函数修饰，提供场景进入与退出的初始化逻辑，以及各个循环过程中的逻辑：

```
#ifndef _SCENE_H_
#define _SCENE_H_

#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {}
	virtual void on_update() {}
	virtual void on_draw() {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_exit() {}

private:

};

#endif
```



以上就类似于场景的模板，所有场景需要继承Scene基类实现虚函数内容即可实现不同场景的不同逻辑部分。

### 场景类具体实例化

接下来开始实现第一个用于实例化的场景子类主菜单场景类，创建头文件menuScene.h使用宏防止重复包含并引入scene.h头文件并使MenuScene继承Scene并重写主菜单对应逻辑方法，为方便测试，当前仅对各部分实现控制台打印文字的效果：

```
#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"

#include <iostream>

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入主菜单！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "主菜单运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"主菜单绘制");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "主菜单退出！" << std::endl;
	}

private:

};

#endif
```



回到main.cpp中着手实例化菜单场景，引入对应头文件，主循环外new创建菜单场景对象，

```
//包含头文件
#include "scene.h"
#include "menuScene.h"

//主循环while之前创建菜单场景对象
Scene* scene = new MenuScene();
scene->on_enter();

//读取操作
scene->on_input(msg);

//更新数据
scene->on_updata();

//绘制图像
scene->on_draw();

```



运行即可看到提示词，接下来开始编写游戏场景类，创建头文件gameScene.h头文件，并类比menuScene.h实现对应功能：

```
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

#include <iostream>

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入游戏局内场景！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "游戏运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"游戏局内绘制");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "游戏局内退出！" << std::endl;
	}

private:

};

#endif
```



接下来考虑场景之间跳转功能，类比舞台剧，不同幕布间的切换需要一位导演记录与管理幕布的切换动作，在程序中，我们称其为场景管理器！

虽然如此多代码与游戏逻辑无关，但这就是大多数人初学时容易忽略的架构意识，如果一个程序在开发初期就没有从分析功能入手设计良好的程序结构，随着功能开发的推进代码间逻辑就会越来越混乱，添加新功能时无从下手之后出现了问题还不容易测试。

## 第一节代码完整展示

**main.cpp**

```
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
```

**scene.h**

```
#ifndef _SCENE_H_
#define _SCENE_H_

#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {}
	virtual void on_update() {}
	virtual void on_draw() {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_exit() {}

private:

};

#endif
```

**menuScene.h**

```
#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"

#include <iostream>

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入主菜单！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "主菜单运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"主菜单绘制");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "主菜单退出！" << std::endl;
	}

private:

};

#endif
```

**gameScene.h**

```
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

#include <iostream>

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入游戏局内场景！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "游戏运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"游戏局内绘制");
	}

	virtual void on_input(const ExMessage& msg) {}

	virtual void on_exit()
	{
		std::cout << "游戏局内退出！" << std::endl;
	}

private:

};

#endif
```



## 第二节

### 场景管理器

项目程序实际上就是一个大大的循环，同时也是一个状态机，而运行过程中的场景就是一幕幕状态，管理这些场景的状态机就是场景管理器。

类比其他类，接下来将场景管理器封装为SceneManager创建于sceneManager.h文件下，场景管理器实际上是对场景切换，就像是一台状态机，从菜单界面到选人界面到游戏界面再到菜单界面，每个界面就是一个状态。

所以类内创建枚举类，SceneType并提供setCurrentState设置当前状态，注意进入场景调用on_enter方法确保场景执行完整，以及switchTo方法执行场景间跳转：

```
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void setCurrentState(Scene* scene)
	{
		this->m_current_scene = scene;
		this->m_current_scene->on_enter();
	}

public:
	Scene* m_current_scene = nullptr;

};

#endif
```

