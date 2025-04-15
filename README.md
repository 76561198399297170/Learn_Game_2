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
#define _ATLAS_H_

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
#define _ATLAS_H_

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

### 图片翻转

大部分包括我们本次项目，角色的左右移动序列帧图像仅仅只是水平镜像的差异，所以本次项目只提供了向右序列帧的，而向左序列帧则可以通过图片像素进行动态生成，和Animation强相关的Atlas类还有一个功能未完成，就是水平翻转，图片像素翻转需要对每个像素逐个处理，是一个相对耗时的操作所以这个操作需要在游戏初始化阶段完成，思路上通过加载图集向右图片遍历每一张并将其拷到向左图集中。

首先定义一个utils.h工具类头文件，编写flipImage翻转图片函数，包含两个参数翻转目标图片与翻转结果图片，借助GetImageBuffer函数对像素进行操作：

```
#ifndef _UTILS_H_
#define _UTILS_H_

#include <graphics.h>

inline void flipImage(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth(), h = src->getheight();
	Resize(dst, w, h);
	
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int src_idx = i * w + j, dst_idx = (i + 1) * w - j - 1;
			dst_buffer[dst_idx] = src_buffer[src_idx];
		}
	}
	return;
}

#endif
```



接下来可以进行数据加载了，在main.cpp中引入utils.h和atlas.h，并定义图集翻转（注意这里是图集，前面的是图片）函数，首先清空目标图集防止复用时出现问题，随后原图集中每一帧图片都执行flipImage操作并提供addImage添加图集：

```
#include "atlas.h"
#include "utils.h"

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
```



### 图像加载

接下来可以将素材中内容放到项目工程中的resources目录下，并在main.cpp下编写loadGameResources函数加载资源，由于本项目较小，所以为方便后续开发过程中直接使用，而非使用时跳转回来添加新的资源家啊在逻辑，目前阶段选择将整个程序所需资源全部加载。

首先是所需的图片图集变量定义在全局环境并注意起名（最好有意义并且包含未知朝向等信息），对应的在loadAGameResources函数中进行加载：

```
//因使用了mciSendString所以全局区需要包含对应库
#pragma comment(lib, "Winmm.lib")

IMAGE img_menu_background;					//主菜单背景图片

IMAGE img_VS;								//VS 艺术字图片
IMAGE img_1P;								//1P 文本图片
IMAGE img_2P;								//2P 文本图片
IMAGE img_1P_desc;							//1P 键位描述图片
IMAGE img_2P_desc;							//2P 键位描述图片
IMAGE img_select_background__left;			//选人朝左背景图片
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
IMAGE img_gamer1_selector_background_right;	//选人界面类型1朝右北京图片
IMAGE img_gamer2_selector_background_left;	//选人界面类型2朝左背景图片
IMAGE img_gamer2_selector_background_right;	//选人界面类型2朝右北京图片

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

void loadGameResources()
{
	AddFontResourceEx(L"./resources/HYPixel11pxU-2.ttf", FR_PRIVATE, NULL);

	loadimage(&img_menu_background, L"./resources/menu_background.png");
	loadimage(&img_1P, L"./resources/1P.png");
	loadimage(&img_2P, L"./resources/2P.png");
	loadimage(&img_1P_desc, L"./resources/1P_dest.png");
	loadimage(&img_2P_desc, L"./resources/2P_dest.png");
	loadimage(&img_select_background_right, L"./resources/select_background.png");
	flipImage(&img_select_background_right, &img_select_background_left);
	loadimage(&img_selector_tip, L"./resources/selector_tip.png");
	loadimage(&img_selector_background, L"./resoources/selector_background.png");
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

	loadimage(&img_gamer1_bullet, L"./resources/gamer_bullet.png");
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

//在加载资源处注意调用该函数

```



编写代码时（除非是新手为了熟悉关键字）应当学会借助编译器代码提示工具补全变量，提高开发效率。

### 动画类实现

创建Animation.h文件以及动画类，然后依旧是考虑成员变量与成员函数，成员变量一般决定这个类的数据属性，而成员函数则是根据这些数据属性选择对外提供何种的增删改查接口。

正如开始的设计数量动画类必然需要持有图集Atlas引用，所以引入atlas.h文件并声明类内私有成员Atlas指针默认nullptr，随后添加各种成员变量如：计时器，帧间隔，帧索引与动画是否循环标记。

成员函数方面提供reset函数将计时器与下标索引重置，setAtlas设置图集方法，setLoop设置是否循环播放，setInterval设置帧间隔，getIndexFrame获取当前播放序列帧下标，getFrame获取当前播放动画帧图片，checkFinished查看是否结束播放的方法。

最后就是动画更新逻辑部分，定义on_update更新方法逻辑与之前基本相同，以及on_draw方法，不过需要先在utils.h中编写putImageAlpha方法解决putImage方法不接受透明度通道的情况，记得需要添加MSIMG32.LIB库。

```
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "atlas.h"
#include "utiles.h

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void reset()
	{
		this->m_timer = 0;
		this->m_idx_frame = 0;
	}

	void setAtlas(Atlas* new_atlas)
	{
		this->reset();
		this->m_atlas = new_atlas;
	}

	void setLoop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void setInterval(int ms)
	{
		this->m_interval = ms;
	}

	int getIndexFrame()
	{
		return this->m_idx_frame;
	}

	IMAGE* getFrame()
	{
		return this->m_atlas->getImage(this->m_idx_frame);
	}

	bool checkFinished()
	{
		if (this->is_loop) return false;

		return this->m_idx_frame == this->m_atlas->getSize() - 1;
	}

	void on_update(int delta)
	{
		this->m_timer += delta;
		if (this->m_timer >= this->m_interval)
		{
			this->m_timer = 0;
			this->m_idx_frame++;
			if (this->m_idx_frame >= this->m_atlas->getSize())
			{
				this->m_idx_frame = (this->is_loop ? 0 : this->m_atlas->getSize() - 1);
			}
		}
	}

	void on_draw(int x, int y) const
	{
		putImageAlpha(x, y, this->m_atlas->getImage(this->m_idx_frame));
	}

private:
	int m_timer = 0;
	int m_interval = 0;
	int m_idx_frame = 0;

	bool is_loop = true;

	Atlas* m_atlas = nullptr;

};

#endif

//utils.h下添加编写putImageAlpha方法，基本不变
#pragma comment(lib, "MSIMG32.LIB")

void putImageAlpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

```



### 生命周期结束动画播放逻辑

在生命周期结束时消失动画通常较为常用，这部分的逻辑又该如何编写？想要播放死亡动画但由于敌人死亡时直接删除了Enemy对象导致无法正常播放，解决方法很简单关键在于延后这些存在消失动画被删除的时间，即物品“死亡”时播放死亡动画，死亡动画播放结束再删除对象。

这就需要动画层面提供一个动画播放结束的消息，当然也可以在动画更新时使用checkFinished方法检查是否播放结束，但这里右更优雅更好的方式即回调函数，即使用参数传递的函数对象使用变量存储，再在合适时调用，例如将删除对象的方法保存，在死亡动画播放完毕时调用回调函数将对象删除来实现我们的目的。

在animation.h中添加头文件functional并在成员变量中添加std::function\<void\(\)\> m_callback变量，随后提供setCallback函数设置方法，并修改on_updata中逻辑部分：

```
//头文件
#include <functional>

//Animation类内成员变量callback回调函数
std::function<void()> m_callback;

//Animation类内成员函数setCallback方法
void setCallBack(std::function<void()> callback)
{
    this->m_callback = callback;
}

//添加下标越界后的询问是否调用回调函数
if (!this->is_loop && this->m_callback)
{
    this->m_callback();
}
```



## 第三节代码完成展示

**main.cpp**

```
#include "scene.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"
#include "sceneManager.h"
#include "atlas.h"
#include "utils.h"

#pragma comment(lib, "Winmm.lib")

#include <graphics.h>

const int FPS = 60;

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
	loadimage(&img_1P, L"./resources/1P.png");
	loadimage(&img_2P, L"./resources/2P.png");
	loadimage(&img_1P_desc, L"./resources/1P_dest.png");
	loadimage(&img_2P_desc, L"./resources/2P_dest.png");
	loadimage(&img_select_background_right, L"./resources/select_background.png");
	flipImage(&img_select_background_right, &img_select_background_left);
	loadimage(&img_selector_tip, L"./resources/selector_tip.png");
	loadimage(&img_selector_background, L"./resoources/selector_background.png");
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

	loadimage(&img_gamer1_bullet, L"./resources/gamer_bullet.png");
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

**utils.h**

```
#ifndef _UTILS_H_
#define _UTILS_H_

#pragma comment(lib, "MSIMG32.LIB")

#include <graphics.h>

void putImageAlpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void flipImage(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth(), h = src->getheight();
	Resize(dst, w, h);
	
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int src_idx = i * w + j, dst_idx = (i + 1) * w - j - 1;
			dst_buffer[dst_idx] = src_buffer[src_idx];
		}
	}
	return;
}

#endif
```

**animation.h**

```
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>

#include "atlas.h"
#include "utils.h"

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void reset()
	{
		this->m_timer = 0;
		this->m_idx_frame = 0;
	}

	void setAtlas(Atlas* new_atlas)
	{
		this->reset();
		this->m_atlas = new_atlas;
	}

	void setLoop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void setInterval(int ms)
	{
		this->m_interval = ms;
	}

	int getIndexFrame()
	{
		return this->m_idx_frame;
	}

	IMAGE* getFrame()
	{
		return this->m_atlas->getImage(this->m_idx_frame);
	}

	bool checkFinished()
	{
		if (this->is_loop) return false;

		return this->m_idx_frame == this->m_atlas->getSize() - 1;
	}

	void setCallBack(std::function<void()> callback)
	{
		this->m_callback = callback;
	}

	void on_update(int delta)
	{
		this->m_timer += delta;
		if (this->m_timer >= this->m_interval)
		{
			this->m_timer = 0;
			this->m_idx_frame++;
			if (this->m_idx_frame >= this->m_atlas->getSize())
			{
				this->m_idx_frame = (this->is_loop ? 0 : this->m_atlas->getSize() - 1);
				if (!this->is_loop && this->m_callback)
				{
					this->m_callback();
				}
			}
		}
	}

	void on_draw(int x, int y) const
	{
		putImageAlpha(x, y, this->m_atlas->getImage(this->m_idx_frame));
	}

private:
	int m_timer = 0;
	int m_interval = 0;
	int m_idx_frame = 0;

	bool is_loop = true;

	Atlas* m_atlas = nullptr;

	std::function<void()> m_callback;

};

#endif
```



## 第四节

游戏设计领域有一个很著名的概念叫"3C"，其几乎囊括了大部分游戏设计中最基本的三种元素角色Character，摄像机Camera，控制Control，决定视野与观察世界的核心元素就是Camera摄像机，为了让画面更灵活自然增强世界感，由此我们第四节进行讨论如何实现2D摄像机的基础。

虽然在之前项目中以及可以使用动画框架顺利进行渲染了，但摄像机作为整个框架中极为重要的一环，还是需要将这个部分集成到地产代码中去再开始更上层的主体逻辑开发。

### 3C——Camera摄像头概念

以超级马里奥为例，摄像头始终跟随马里奥，当马里奥向右移动实际相对的其他元素则是向左移动，也就是当整个背景向左移动，玩家也就向右移动了，也就是以玩家为参考系，移动玩家也就是移动除玩家外的所有对象，但是移动全体对象的话，碰撞检测与坐标相关问题该如何解决呢？

此时应引入两个重要概念进行比对，“窗口坐标系”与“世界坐标系”，窗口坐标系的概念我们一致都在使用还是比较熟悉了，在EasyX默认窗口坐标系就是左上角作为原点水平向右与竖直向下为x轴正方向与y轴正方向的坐标系，而世界坐标系就是更大更广阔的坐标系，任何的一切都是在世界坐标系下运转也就是只有在需要绘制时我们才考虑将他们防止到窗口坐标系下进行绘制等操作（比如Minecraft中打开F4看到的整个世界的坐标系，而玩家作为原点，加载范围左上角与右下角为终点的坐标系就是窗口坐标系），摄像机是世界坐标系与窗口坐标系之间转换的利器。

以上的设计思路正好符合最开始的设计理念，也就是数据与渲染分离，在不考虑缩放的情况下，如果摄像头画面与窗口画面大小一致时，摄像头可以看作在世界坐标系中的一个点，且这个点与玩家坐标位置始终保持一致，渲染场景中其他物体只需要将其他物体世界坐标与当前摄像机点位坐标做差得出位置，就是实际渲染位置：窗口坐标 = 世界坐标 - 摄像机坐标。

不过在编写Camera之前，我需要先对之前的动画类功能测试，以便摄像头类中可以使用。

### 动画类的测试与使用

首先来到menuScene.h中，引入atlas.h头文件与animation.h并使用extern声明外部动画图集，随后在MenuScene私有成员中定义Animation对象m_animation_gamer1_run_right并在成员函数on_enter中设置所使用的图集，帧间隔与循环状态

```
//头文件引入及外部变量引入，注意这里的顺序不然下面会报错
#include "atlas.h"
#include "animation.h"
#include "scene.h"
#include "sceneManager.h"

extern Atlas atlas_gamer1_run_right;

//MenuScene私有成员变量
Animation m_animation_gamer1_run_right;

//MenuScene成员函数on_enter初始化，这里如果出现参数不匹配就是头文件顺序需要调整
this->m_animation_gamer1_run_right.setAtlas(&atlas_gamer1_run_right);
this->m_animation_gamer1_run_right.setInterval(75);
this->m_animation_gamer1_run_right.setLoop(true);

```



我们接下来接下来需要进行on_updata编写但是发现，缺少了一个实际运行时间参数的传入，所以此处就需要小幅度重构代码了，首先是场景基类scene.h头文件中，对on_updata方法添加int delta表示实际时间参数随后给menuScene.h，gameScene.h，selectorScene.h中为重构的on_updata添加参数，以及对sceneManager.h中同样添加上on_updata的参数，对main.h中调用on_updata处传入参数即可。

```
//scene.h中重构
virtual void on_update(int delta) {}

//menuScene.h中重构
virtual void on_update(int delta)

//selectorScene.h中重构
virtual void on_update(int delta)

//gameScene.h中重构
virtual void on_update(int delta)

//sceneManager.h中重构
void on_updata(int delta)
{
    this->m_current_scene->on_update(delta);
}

//main.h中重写解决方案，通过getTickCount获取并计算上次调用时间间隔，来传入参数
static DWORD last_tick_time = GetTickCount();
DWORD current_tick_time = GetTickCount();
DWORD delta_tick_time = current_tick_time - last_tick_time;
scene_manager.on_updata(delta_tick_time);
last_tick_time = current_tick_time;

```



这个插曲可以注意到，一开始没有考虑好代码结构的设计是多糟糕的事情，一旦出现错误就需要翻遍整个项目进行修改，虽然对于新手出现类似的问题是正常的情况，属于是缺乏程序设计方面的经验，也是学习过程中必须要经历的问题，积累经验才能学会。

回到menuScene.h头文件的MenuScene类的on_draw方法中调用成员函数animation中的on_draw方法渲染在100，100位置处，运行程序能够正确看到渲染内容即成功。

```
//MenuScene类中修改如下代码
virtual void on_update(int delta)
{
	this->m_animation_gamer1_run_right.on_update(delta);
}

virtual void on_draw()
{
	this->m_animation_gamer1_run_right.on_draw(100, 100);
}
```

