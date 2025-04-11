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

	void switchTo(SceneType type)
	{
		this->m_current_scene->on_exit();
		switch (type)
		{
		case SceneManager::SceneType::Menu:
			break;
		case SceneManager::SceneType::Game:
			break;
		default:
			break;
		}
		this->m_current_scene->on_enter();
	}

public:
	Scene* m_current_scene = nullptr;

};

#endif
```



这里的on_enter和on_exit类似析构函数与构造函数，但是构造函数与析构函数同在决定这场景对象在内存中的生命周期，大项目中不断析构与构造花费大量时间重复进行创建与销毁是不友好的，不同场景间资源也会存在相互引用也就是场景内对象生命周期长于场景对对象本身生命周期的情况，这对内存管理提出了更高要求。

这里就是一个简单的方法，就是将场景对象生命周期与程序生命周期相同，也就是程序运行时创建初始化所有场景对象结束时释放所有场景对象，所以这里避免了析构与构造函数转而提供语义明确的on_enter与on_exit方法并在其中尽可能避免释放资源而是重置其状态。

例如当游戏局内场景中玩家类生命值归零回到主菜单时，原本情况下我们应该调用其析构函数释放资源在重新进入游戏局内时再创建新的玩家类，但是现在我们只需要在on_enter时重置玩家类状态即可。

接下来在main.cpp中创建场景类全局指针，并且在初始化阶段创建实例化：

```
//全局区
Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;

//初始化
menu_scene = new MenuScene();
game_scene = new GameScene();
```



随后在sceneManager.h中通过extern关键字获取到这两个全局指针引用，并将sceneManager成员函数switchTo中跳转方法补全，修改成员函数setCurrentState更名为setCurrentScene，补全成员函数on_input与on_updata与on_draw方法，目前只需要做简单的传递即可：

```
//全局区
extern Scene* menu_scene;
extern Scene* game_scene;

//sceneManager成员函数switchTo中
switch (type)
{
case SceneManager::SceneType::Menu:
	this->m_current_scene = menu_scene;
	break;
case SceneManager::SceneType::Game:
	this->m_current_scene = game_scene;
	break;
default:
	break;
}

//成员函数补齐
void on_updata()
{
    this->m_current_scene->on_update();
}

void on_draw()
{
    this->m_current_scene->on_draw();
}

void on_input(const ExMessage& msg)
{
    this->m_current_scene->on_input(msg);
}
```



为什么场景跳转使用枚举类作为参数而设置跳转则使用场景指针？原因是设置当前场景的方法通常只在启动时初始化，设置场景管理器入口场景时才被调用，与子场景实例化几乎同时进行所以使用指针会更方便，跳转场景基本在不同场景内部更新时被调用，场景间持有彼此引用是一种不太优雅且极其任意出现内存问题的行为，所以使用枚举类来屏蔽掉管理器内部指针操作。

随后在main.cpp中引入sceneManager.h头文件并在全局区声明实例化场景管理器删除mian中之前测试的scene变量，并在初始化设置场景管理器入口场景为menu_scene主菜单场景（注意先后顺序），并在主循环中替换scene的方法：

```
//全局区
SceneManager scene_manager;

//初始化设置入口场景
scene_manager.setCurrentState(menu_scene);

//主循环框架的调用
while (peekmessage(&msg))
{
	scene_manager.on_input(msg);
}
scene_manager.on_updata();

cleardevice();
scene_manager.on_draw();

FlushBatchDraw();
```



接下来便是具体场景间的跳转功能代码了，首先在menuScene.h中引入sceneManager.h头文件并使用extern声明外部的scene_manager对象，若我们希望在主菜单界面按下任意键跳转到Game场景则编写如下代码，并在gameScene.h中编写相似代码，运行程序即可在程序中按任何键在两个界面中来回切换了：

```
//menuScene.h全局区
#include "sceneManager.h"

extern SceneManager scene_manager;

//MenuScene成员函数on_input中
virtual void on_input(const ExMessage& msg)
{
    if (msg.message == WM_KEYDOWN)
    {
        scene_manager.switchTo(SceneManager::SceneType::Game);
    }
}

//gameScene.h全局区
#include "sceneManager.h"

extern SceneManager scene_manager;

//GameScene成员函数on_input中
virtual void on_input(const ExMessage& msg)
{
    if (msg.message == WM_KEYDOWN)
    {
        scene_manager.switchTo(SceneManager::SceneType::Menu);
    }
}
```



正如之前规划的还需要一个选人界面，所以定义selectorScene.h文件并实例化SelectorScene继承Scene基类编写类似代码，并注意在main.cpp中创建并初始化，在sceneManager.h中extern声明并修改枚举类与switchTo，以及修改各个场景间on_input的各场景间切换：

```
//selectorScene.h文件代码
#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入选人！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "选人运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"选人绘制");
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Menu);
		}
	}

	virtual void on_exit()
	{
		std::cout << "选人退出！" << std::endl;
	}

private:

};

#endif

//main.cpp全局声明
#include "selectorScene.h"
Scene* selector_scene = nullptr;

//main.cpp主循环中初始化
selector_scene = new SelectorScene();

//sceneManager.h文件中全局声明并修改枚举类与成员函数switchTo
extern Scene* selector_scene;//全局声明

//成员枚举类
enum class SceneType
{
    Menu,
    Game,
    Selector
};

//成员函数switchTo的switch(type)跳转中添加
case SceneManager::SceneType::Selector:
    this->m_current_scene = selector_scene;
    break;


//menuScene.h成员函数on_input修改
scene_manager.switchTo(SceneManager::SceneType::Selector);

//gameScene.h成员函数on_input修改
scene_manager.switchTo(SceneManager::SceneType::Menu);
```



运行程序，按下任意键即可执行由Menu->Selector->Game->Menu...的场景跳转了，我们也可以在visual studio的左侧解决方案添加筛选器Scene并将场景文件添加到其中，添加筛选器Manager将管理器添加到其中既美观又一目了然方便管理。

### 资源加载

接下来是资源加载部分，在此之前先回顾之前的资源加载中的动画相关概念，在程序中我们通过快速切换一系列图片通过人眼的视觉留影实现动画效果，为了方便资源管理和可复用性，我们设计了Atlas图集类和Animation动画类，Atlas作为有相关性一系列图片的资源管理容器，而Animation作为觉得实际渲染图集的轻量控制器。

首先从atlas.h头文件开始创建Atlas类，由于整体图片资源名称规律，所以提供loadFromFile方法使用模板加载图片资源，以及clear方法，getSize方法，getImage方法，addImage方法，对于图片翻转等功能帮助较大：

```
#ifndef _ATLAS_H_
#define _ATALS_H_

#include <vector>
#include <graphics.h>

class Atlas 
{
public:
	Atlas() = default;
	~Atlas() = default;

	void loadFromFile(LPCTSTR path_template, int num)
	{
		this->m_img_list.clear();
		this->m_img_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&this->m_img_list[i], path_file);
		}
	}

	void clear()
	{
		this->m_img_list.clear();
	}

	int getSize()
	{
		return this->m_img_list.size();
	}

	IMAGE* getImage(int idx)
	{
		if (idx < 0 || idx >= this->m_img_list.size()) return nullptr;

		return &this->m_img_list[idx];
	}

	void addImage(const IMAGE& img)
	{
		this->m_img_list.push_back(img);
	}

private:
	std::vector<IMAGE> m_img_list;

};

#endif
/*
本段注释不包含在文件内。
若需要加载根目录img文件下的img_1.png，img_2.png，img_3.png，img_4.png图集可以编写如下的代码：
Atlas atlas;
atlas.loadFromFile(L"./img/img_%d.png", 4);
*/
```



底层的Atlas代码以及完成，而Animation类都是在Atlas上进行实现的，动画效果还需待Animation类完成，同时动画的更新与渲染也对现有的场景架构提出了新的要求。

## 第二节代码完成展示

**main.cpp**

```
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
```

**atlas.h**

```
#ifndef _ATLAS_H_
#define _ATALS_H_

#include <vector>
#include <graphics.h>

class Atlas 
{
public:
	Atlas() = default;
	~Atlas() = default;

	void loadFromFile(LPCTSTR path_template, int num)
	{
		this->m_img_list.clear();
		this->m_img_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&this->m_img_list[i], path_file);
		}
	}

	void clear()
	{
		this->m_img_list.clear();
	}

	int getSize()
	{
		return this->m_img_list.size();
	}

	IMAGE* getImage(int idx)
	{
		if (idx < 0 || idx >= this->m_img_list.size()) return nullptr;

		return &this->m_img_list[idx];
	}

	void addImage(const IMAGE& img)
	{
		this->m_img_list.push_back(img);
	}

private:
	std::vector<IMAGE> m_img_list;

};

#endif
```

**gameScene.h**

```
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

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

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Menu);
		}
	}

	virtual void on_exit()
	{
		std::cout << "游戏局内退出！" << std::endl;
	}

private:

};

#endif
```

**menuScene.h**

```
#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

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

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Selector);
		}
	}

	virtual void on_exit()
	{
		std::cout << "主菜单退出！" << std::endl;
	}

private:

};

#endif
```

**selectorScene.h**

```
#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	virtual void on_enter()
	{
		std::cout << "进入选人！" << std::endl;
	}

	virtual void on_update()
	{
		std::cout << "选人运行中..." << std::endl;
	}

	virtual void on_draw()
	{
		outtextxy(10, 10, L"选人绘制");
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switchTo(SceneManager::SceneType::Game);
		}
	}

	virtual void on_exit()
	{
		std::cout << "选人退出！" << std::endl;
	}

private:

};

#endif
```

**sceneManager.h**

```
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void setCurrentState(Scene* scene)
	{
		this->m_current_scene = scene;
		this->m_current_scene->on_enter();
	}

	void switchTo(SceneType type)
	{
		this->m_current_scene->on_exit();
		switch (type)
		{
		case SceneManager::SceneType::Menu:
			this->m_current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			this->m_current_scene = game_scene;
			break;
		case SceneManager::SceneType::Selector:
			this->m_current_scene = selector_scene;
			break;
		default:
			break;
		}
		this->m_current_scene->on_enter();
	}

	void on_updata()
	{
		this->m_current_scene->on_update();
	}

	void on_draw()
	{
		this->m_current_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		this->m_current_scene->on_input(msg);
	}

public:
	Scene* m_current_scene = nullptr;

};

#endif
```



## 第三节
