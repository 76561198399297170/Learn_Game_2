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



以及测试一下动画播放结束的回调函数，对menuScene中类内的on_enter修改，增加设置回调函数且使用lambda匿名函数的方式，运行结果可以正确执行跳转至此测试完毕：

```
this->m_animation_gamer1_run_right.setLoop(false);
this->m_animation_gamer1_run_right.setCallBack(
	[]()
	{
		scene_manager.switchTo(SceneManager::SceneType::Game);
	}
);
```



### Camera核心代码

前面探讨过程中，基本需求可以得知摄像机基本可以等效为一个点，虽然可以直接只有POINT来记录整个点，但是为了使用浮点更精细记录这个点我们需要建立在游戏框架中常见的二维向量类。

首先创建头文件vector2.h文件并定义Vector2类，添加公有成员浮点横纵坐标并编写有参构造方法，随后编写后续可能需要的运算符重载方便后续使用，最后添加两个成员函数获取斜边长度即向量长度与获取向量标准化方法：

```
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y) : m_x(x), m_y(y) {}

	Vector2 operator+(const Vector2& vec)
	{
		return Vector2(this->m_x + vec.m_x, this->m_y + vec.m_y);
	}

	void operator+=(const Vector2& vec)
	{
		this->m_x += vec.m_x, this->m_y += vec.m_y;
	}

	Vector2 operator-(const Vector2& vec)
	{
		return Vector2(this->m_x - vec.m_x, this->m_y - vec.m_y);
	}

	void operator-=(const Vector2& vec)
	{
		this->m_x -= vec.m_x, this->m_y -= vec.m_y;
	}

	float operator*(const Vector2& vec)
	{
		return this->m_x * vec.m_x + this->m_y * vec.m_y;
	}

	Vector2 operator* (const float val) const
	{
		return Vector2(this->m_x * val, this->m_y * val);
	}

	void operator*=(float val)
	{
		this->m_x *= val, this->m_y *= val;
	}

	float length()
	{
		return sqrt(pow(this->m_x, 2) + pow(this->m_y, 2));
	}

	Vector2 normalize()
	{
		float len = this->length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(this->m_x / len, this->m_y / len);
	}

public:
	float m_x;
	float m_y;

};

#endif
```



接下来创建camera.h头文件定义Camera摄像机类，并在引入vector2.h头文件后创建私有成员变量m_position以及getPosition方法获取引用常量，提供reset方法使得坐标归零，声明on_updata方法因暂无需要添加的内容让其暂时空置：

```
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector2.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void reset()
	{
		this->m_position.m_y = this->m_position.m_x = 0;
	}

	const Vector2& getPosition() const
	{
		return this->m_position;
	}

	void on_updata(int delta)
	{

	}

private:
	Vector2 m_position;

};

#endif
```



### 摄像头功能测试

来到menuScene.h文件下，为观感更好，取消之前单词播放与回调函数并引入camera.h文件定义私有成员Camera并在on_draw中修改绘制方式，运行可以看到与之前相同的结果：

```
//头文件引入
#include "camera.h"

//MenuScene类内成员变量声明
Camera m_camera;

//修改成员函数on_enter
virtual void on_enter()
{
    this->m_animation_gamer1_run_right.setAtlas(&atlas_gamer1_run_right);
    this->m_animation_gamer1_run_right.setInterval(75);
    this->m_animation_gamer1_run_right.setLoop(true);
}

//修改成员函数on_draw
virtual void on_draw()
{
    const Vector2& pos_camera = this->m_camera.getPosition();
    this->m_animation_gamer1_run_right.on_draw((int)(100 - pos_camera.m_x), (int)(100 - pos_camera.m_y));
}

```



因为此时，摄像头的世界坐标是（0, 0）而渲染的动画坐标在（100, 100）此时做差与原先位置不变，想要确定功能实现的话我们应该尝试让摄像头动起来，首先需要在MenuScene中的on_updata调用Camera的on_updata方法，随后回到camera.h中修改on_updata方法编写如下逻辑再运行程序，可以看到渲染的动画水平向右移动，正是因为摄像头x坐标变小（摄像头向左水平移动）导致的，由此摄像头的基础完成：

```
void on_updata(int delta)
{
	const Vector2 speed = { -0.35f, 0 };
	this->m_position += speed * (float)delta;
}
```



## 第四节代码完成展示

**main.cpp**

```
#include "atlas.h"

#include "scene.h"
#include "sceneManager.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"

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

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		scene_manager.on_updata(delta_tick_time);
		last_tick_time = current_tick_time;

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
	virtual void on_update(int delta) {}
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
#include "sceneManager.h"

#include "animation.h"
#include "camera.h"

#include <iostream>

extern Atlas atlas_gamer1_run_right;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		this->m_animation_gamer1_run_right.setAtlas(&atlas_gamer1_run_right);
		this->m_animation_gamer1_run_right.setInterval(75);
		this->m_animation_gamer1_run_right.setLoop(true);
	}

	virtual void on_update(int delta)
	{
		this->m_camera.on_updata(delta);
		this->m_animation_gamer1_run_right.on_update(delta);
	}

	virtual void on_draw()
	{
		const Vector2& pos_camera = this->m_camera.getPosition();
		this->m_animation_gamer1_run_right.on_draw((int)(100 - pos_camera.m_x), (int)(100 - pos_camera.m_y));
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
	Animation m_animation_gamer1_run_right;
	Camera m_camera;

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

	virtual void on_update(int delta)
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

	virtual void on_update(int delta)
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

**vector2.h**

```
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y) : m_x(x), m_y(y) {}

	Vector2 operator+(const Vector2& vec)
	{
		return Vector2(this->m_x + vec.m_x, this->m_y + vec.m_y);
	}

	void operator+=(const Vector2& vec)
	{
		this->m_x += vec.m_x, this->m_y += vec.m_y;
	}

	Vector2 operator-(const Vector2& vec)
	{
		return Vector2(this->m_x - vec.m_x, this->m_y - vec.m_y);
	}

	void operator-=(const Vector2& vec)
	{
		this->m_x -= vec.m_x, this->m_y -= vec.m_y;
	}

	float operator*(const Vector2& vec)
	{
		return this->m_x * vec.m_x + this->m_y * vec.m_y;
	}

	Vector2 operator*(const float val) const
	{
		return Vector2(this->m_x * val, this->m_y * val);
	}

	void operator*=(float val)
	{
		this->m_x *= val, this->m_y *= val;
	}

	float length()
	{
		return sqrt(pow(this->m_x, 2) + pow(this->m_y, 2));
	}

	Vector2 normalize()
	{
		float len = this->length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(this->m_x / len, this->m_y / len);
	}

public:
	float m_x;
	float m_y;

};

#endif
```

**camera.h**

```
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector2.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void reset()
	{
		this->m_position.m_y = this->m_position.m_x = 0;
	}

	const Vector2& getPosition() const
	{
		return this->m_position;
	}

	void on_updata(int delta)
	{
		const Vector2 speed = { -0.35f, 0 };
		this->m_position += speed * (float)delta;
	}

private:
	Vector2 m_position;

};

#endif
```



## 第五节

### 抖动效果引入

游戏的画面不一定是素材的累积，很多时候质感有也可以通过一些小技巧快速提升，这一节将在之前Camera基础上实现摄像机抖动的效果，很多时候开发者会选择让屏幕以不同幅度的抖动来透过屏幕表现打击的力量感，可以说这是一种实现起来相对廉价的但表现不错的视觉特效。

### 计时器概念

摄像机的抖动特效作为玩家画面的一个限时状态，必然要在开始的一段时间后结束这种效果，所以和之前的播控相似，也需要一个定时器来控制特效的开始与结束时刻，并且从长远的角度看，技能CD或无敌帧等等效果都需要时间相关的功能，所以不如先封装一个通用定时器来对这些有时效性的功能提供相对统一的管理模式。

关于实现部分还是需要考虑其设计思想的，这里有两种定时器设计思路，一是继承二回调；

  **·**  继承在on_updata中指向定时器时间到达时的逻辑，这部分逻辑封装在callback成员方法中，如果想要实现自己的定时器就要实现自己的定时器逻辑，那么只需让自己的定时器集成自Timer基类并重写callback方法，在使用时通过多态便能执行重写后的定时器逻辑。

  **·**  回调与继承的逻辑相似，通过setCallback成员方法，讲回调函数保存在对象内部并在合适时进行调用，使用时只需向对象注册自己的回调函数即可。

对比两种定时器实现方式，明显实现回调函数的定时器在代码编写上更为简洁，如果需要不同逻辑的定时器，在继承的方法上就需要编写很多不同的计时器子类，而回调的方法中只需要实例化计时器并编写一个lambda函数即可。

而从代码设计的思路，像通用计时器这类只需要扩展回调方法逻辑而无需扩展数据成员内容的类，我们会更倾向于使用回调函数思路而非使用类的继承，在代码编写起来更加轻松，并且语义上会更加明确。

### 计时器类实现

创建文件Timer.h头文件创建Timer类，声明六项私有成员变量并设计成员方法，提供了reset重置计时器时钟，setWaitTime设置等待时间，setOneShot设置是否单次触发，setCallback设置回调函数，pause使计时器暂停计时，resume使计时器继续进行计时， 最后是核心的更新逻辑on_updata，首先判断是否暂停如果暂停则直接返回，随后对过去时间累加更新帧时间并将计时器启动以来过去的时间与期望时间比较，尝试触发定时器回调函数，注意要检查是否单次触发与是否被触发与callback是否有效才触发callback回调函数，最后进行标记并清空计时：

```
#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;
	
	void restart()
	{
		this->m_pass_time = 0;
		this->is_shotted = false;
	}

	void setWaitTime(int wait_time)
	{
		this->m_wait_time = wait_time;
	}

	void setOneShot(bool is_one_shot)
	{
		this->is_one_shot = is_one_shot;
	}

	void setCallback(std::function<void()> callback)
	{
		this->m_callback = callback;
	}

	void pause()
	{
		this->is_paused = true;
	}

	void resume()
	{
		this->is_paused = false;
	}

	void on_updata(int delta)
	{
		if (!this->is_paused)
		{
			this->m_pass_time += delta;
			if (this->m_pass_time >= this->m_wait_time) 
			{
				if ((!this->is_one_shot || (this->is_one_shot && !this->is_shotted)) && this->m_callback) this->m_callback();

				this->is_shotted = true;
				this->m_pass_time = 0;
			}
		}
	}

private:
	int m_pass_time = 0;//已过时间
	int m_wait_time = 0;//等待时间
	bool is_paused = false;//是否暂停
	bool is_shotted = false;//是否触发
	bool is_one_shot = false;//是否单次触发
	std::function<void()> m_callback;//回调函数

};

#endif
```



代码完成，我们应该开始继承到摄像机类中吗？不，在前面的学习部分已进行了说明，每当我们完成一个功能模块的开发应首要完成的是对其进行测试，避免代码错误累加导致调式困难。

来到menuScene.h头文件中引入timer.h头文件并在私有成员中定义Timer对象在on_enter中对非法进行设置，在on_updata中对方法进行更新，运行程序可以看到命令行每隔一秒重复输出"WOOF!!"即正确完成：

```
//成员变量声明
Timer m_timer;

//成员函数on_enter进行初始化
this->m_timer.setWaitTime(1000);
this->m_timer.setOneShot(false);
this->m_timer.setCallback(
    []()
    {
        std::cout << "WOOF!!" << std::endl;
    }
);

//成员函数on_updata更新计时器
this->m_timer.on_updata(delta);
```



### 摄像机抖动

首先先考虑摄像头抖动效果应如何设计，在前面的设计中我们将摄像头看作世界坐标与窗口坐标的转换器，即将世界坐标与摄像头坐标做差即可得到渲染图形坐标，如果需要将屏幕中所有内容进行抖动就可以改为快速晃动摄像头即可，即在短时间内快速改变摄像头坐标就行，不过晃动方式不同呈现的效果也会不同，一种简单的方法是在以抖动强度为半径的院内随机设置摄像头的位置，因为帧更新速度很快所以在宏观上可以做到抖动效果，如果想要抖动更加平滑可以使用柏林噪音生成的随机数取代简单的随机数生成，不过这就涉及到深层的算法问题了，且本项目摄像机小幅度抖动使用柏林噪声优化提升不大，所以此处不多做涉及仅使用简单的随机数生成即可。

接下来就可以将计时器集成到摄像头中了，打开camera.h文件并引入timer.h头文件，扩展三个成员变量计时器，是否抖动中和抖动幅度用于控制摄像头抖动效果，随后，为了启用抖动效果提供shake成员函数设置启动抖动并设置强度与抖动时长，最后修改on_updata中内容更新计时器并根据状态进行抖动，并且我们希望测试其是否成功完成，打开menuScene.h头文件在on_input中编写在按下空格时启动抖动，运行程序发现成功抖动完成：

```
//camera.h成员变量声明
Timer m_shake_timer;
bool is_shaking = false;
float m_shaking_strength = 0;

//camera.h构造函数初始化计时器
this->m_shake_timer.setOneShot(true);
this->m_shake_timer.setCallback(
	[&]()
	{
		this->is_shaking = false;
		this->restart();
	}
);

//camera.h成员函数shake编写
void shake(float strength, int duration)
{
	this->is_shaking = true;
	this->m_shaking_strength = strength;

	this->m_shake_timer.setWaitTime(duration);
	this->m_shake_timer.restart();
}

//camera.h修改成员函数on_updata内容
this->m_shake_timer.on_updata(delta);

if (this->is_shaking)
{
	float radius = this->m_shaking_strength;
	float angle = (float)(rand() % 360) * 3.141592f / 180.0f;

	this->m_position.m_x = radius * cos(angle);
	this->m_position.m_y = radius * sin(angle);
}

//menuScene.h中修改on_input成员函数
if (msg.message == WM_KEYDOWN)
{
    if (msg.vkcode == VK_SPACE) this->m_camera.shake(10, 350);
    else scene_manager.switchTo(SceneManager::SceneType::Selector);
}
```



## 第五节代码完成展示

**menuScene.h**

```
#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "sceneManager.h"

#include "timer.h"
#include "animation.h"
#include "camera.h"

#include <iostream>

extern Atlas atlas_gamer1_run_right;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
		this->m_animation_gamer1_run_right.setAtlas(&atlas_gamer1_run_right);
		this->m_animation_gamer1_run_right.setInterval(75);
		this->m_animation_gamer1_run_right.setLoop(true);
		
		this->m_timer.setWaitTime(1000);
		this->m_timer.setOneShot(false);
		this->m_timer.setCallback(
			[]()
			{
				std::cout << "WOOF!!" << std::endl;
			}
		);
	}

	virtual void on_update(int delta)
	{
		this->m_timer.on_updata(delta);
		this->m_camera.on_updata(delta);
		this->m_animation_gamer1_run_right.on_update(delta);
	}

	virtual void on_draw()
	{
		const Vector2& pos_camera = this->m_camera.getPosition();
		this->m_animation_gamer1_run_right.on_draw((int)(100 - pos_camera.m_x), (int)(100 - pos_camera.m_y));
	}

	virtual void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_SPACE) this->m_camera.shake(10, 350);
			else scene_manager.switchTo(SceneManager::SceneType::Selector);
		}
	}

	virtual void on_exit()
	{
		std::cout << "主菜单退出！" << std::endl;
	}

private:
	Animation m_animation_gamer1_run_right;
	Camera m_camera;
	Timer m_timer;

};

#endif
```

**camera.h**

```
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "timer.h"
#include "vector2.h"

class Camera
{
public:
	Camera()
	{
		this->m_shake_timer.setOneShot(true);
		this->m_shake_timer.setCallback(
			[&]()
			{
				this->is_shaking = false;
				this->reset();
			}
		);
	}

	~Camera() = default;

	void reset()
	{
		this->m_position.m_y = this->m_position.m_x = 0;
	}

	const Vector2& getPosition() const
	{
		return this->m_position;
	}

	void on_updata(int delta)
	{
		this->m_shake_timer.on_updata(delta);

		if (this->is_shaking)
		{
			float radius = this->m_shaking_strength;
			float angle = (float)(rand() % 360) * 3.141592f / 180.0f;

			this->m_position.m_x = radius * cos(angle);
			this->m_position.m_y = radius * sin(angle);
		}
	}

	void shake(float strength, int duration)
	{
		this->is_shaking = true;
		this->m_shaking_strength = strength;

		this->m_shake_timer.setWaitTime(duration);
		this->m_shake_timer.restart();
	}

private:
	Vector2 m_position;
	
	Timer m_shake_timer;
	bool is_shaking = false;
	float m_shaking_strength = 0;

};

#endif
```

**timer.h**

```
#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;
	
	void restart()
	{
		this->m_pass_time = 0;
		this->is_shotted = false;
	}

	void setWaitTime(int wait_time)
	{
		this->m_wait_time = wait_time;
	}

	void setOneShot(bool is_one_shot)
	{
		this->is_one_shot = is_one_shot;
	}

	void setCallback(std::function<void()> callback)
	{
		this->m_callback = callback;
	}

	void pause()
	{
		this->is_paused = true;
	}

	void resume()
	{
		this->is_paused = false;
	}

	void on_updata(int delta)
	{
		if (!this->is_paused)
		{
			this->m_pass_time += delta;
			if (this->m_pass_time >= this->m_wait_time) 
			{
				if ((!this->is_one_shot || (this->is_one_shot && !this->is_shotted)) && this->m_callback) this->m_callback();

				this->is_shotted = true;
				this->m_pass_time = 0;
			}
		}
	}

private:
	int m_pass_time = 0;//已过时间
	int m_wait_time = 0;//等待时间
	bool is_paused = false;//是否暂停
	bool is_shotted = false;//是否触发
	bool is_one_shot = false;//是否单次触发
	std::function<void()> m_callback;//回调函数

};

#endif
```



## 第六节

### 摄像机实例化与使用

接下来我们需要思考一个渲染相关的问题，前完成了摄像机相关的部分内容，接下来我们需要考虑控制游戏的“主摄像机”对象应该声明在何处？正如之前测试时的动画渲染效果我们需要确保每一个场景在执行渲染时都可以获取到摄像机对象从而根据位置进行实际画面渲染。

思路一是定义在场景内部作为场景内成员对象使用，但是不同场景间的摄像机数据就很难进行共享了。

思路二是定义为全局变量和图片资源一样使用extern关键字获取，但全局变量终归是不美观，在项目编写时我们使用面向对象思想一步步对数据进行封装，其中一个很重要的思想就是尽可能去减少全局变量的使用。

那么我们还能使用哪种设计呢，在on_updata时我们使用传参的方法将delta传入，我们也可以将Camera作为参数在对象渲染时通过参数传递使用，只不过这就需要我们再次重构代码了。

首先是scene.h中为on_draw添加参数const Camera& camera，并将其子类对应修改相应重写函数：

```
//scene.h文件添加头文件
#include "camera.h"

//scene.h文件Scene类重写on_draw方法
virtual void on_draw(const Camera& camera) {}

//menuScene.h文件类重写on_draw方法
virtual void on_draw(const Camera& camera)

//selectorScene.h文件类重写on_draw方法
virtual void on_draw(const Camera& camera)

//gameScene.h文件类重写on_draw方法
virtual void on_draw(const Camera& camera)
```



既然所有场景我们都可以通过参数传递获取到摄像机，那么我们就需要再场景管理器绘图阶段将使用的摄像机传入当前场景，也就是修改SceneManager中的on_draw方法修改，随后将main.cpp全局区声明摄像机并在主循环内渲染阶段将摄像机对象传递给场景管理器：

```
//sceneManager.h修改成员函数
void on_draw(const Camera& camera)
{
	this->m_current_scene->on_draw(camera);
}

//main.cpp全局变量
#include "camera.h"
Camera main_camera;

//main.cpp渲染部分修改
scene_manager.on_draw(main_camera);

```



虽然摄像机已经继承到框架中了，但是似乎还是没有避免使用全局变量？虽然在渲染方法的调用过程中只使用了摄像机的位置数据，但摄像机的其他功能例如抖动效果依旧需要编写到渲染方法外，也就是说由于我们没有提供在运行时动态检索游戏对象的设计，所以依然保有部分全局变量也是无奈之举。

不过随着项目的深入，在使用GameObject将整个项目大统一时，我们项目的整个设计将会更加优雅。



### 菜单场景编写

接下来逐步填充各个场景，首先是菜单场景，先前为测试编写了很多无关的代码，首先就需要清空这些代码，将MenuScene类，SelectorScene类，GameScene类的成员变量与on相关方法内部代码清空例如下：

```
#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	virtual void on_enter()
	{
	}

	virtual void on_update(int delta)
	{
	}

	virtual void on_draw(const Camera& camera)
	{
	}

	virtual void on_input(const ExMessage& msg)
	{
	}

	virtual void on_exit()
	{
	}

private:

};

#endif
```



随后我们开始填充MenuScene代码，首先使用extern关键字引入主菜单场景所需的背景资源，随后编写on_draw中使用putimage方法将背景渲染到窗口中，不过为了确保多态接口的一致性on_draw参数不必进行修改，并在on_enter方法中播放背景音乐，最后在on_input中编写按下任意键跳转场景的逻辑：

```
//全局区声明资源
#include "sceneManager.h"

extern SceneManager scene_manager;
extern IMAGE img_menu_background;

//成员函数on_draw方法
putimage(0, 0, &img_menu_background);

//成员函数on_enter方法
mciSendString(L"play bgm_menu repeat from 0", NULL, 0, NULL);

//成员函数ono_input方法
if (msg.message == WM_KEYUP)
{
    mciSendString(L"play ui_confirm repeat from 0", NULL, 0, NULL);
	scene_manager.switchTo(SceneManager::SceneType::Selector);
}
```



### 选角场景编写

接下来开始编写角色选择界面的逻辑，首先引入atlas.h头文件并使用extern声明所需图集与图片资源，为了将资源渲染在正确位置，我们在类内定义一系列POINT类型变量并在on_enter进行初始化，这里并未是直接使用屏幕宽高而是表达式的方法就是为了调整起来方便，并且含义更加明确，对于界面中各类元素位置描述有一套居中外边距等属性的布局框架，在实现该部分内容会更为轻松，但布局框架同样是一个讨论起来费事费力且内容极多的部分，由于本项目只有小部分涉及界面布局若深入讨论布局框架则会耽误太多与主线任务无关的时间，在前面底层搭建阶段已经开辟足够多分枝了所以这种基于布局框架的界面元素定位暂时则不深入讨论。

注意在EasyX中，我们可以使用getwidth与getheight方法获取绘图窗口的宽度和高度，图片类内也提供了获取图片尺寸宽高的方法，由于图片渲染时描述图片位置坐标为图片矩形左上角，如果想让图片在窗口居中则需要在水平与竖直方向上将窗口尺寸和图片尺寸作差并除以二即可，on_enter初始化完成后就可以来到on_draw方法内部将图片内容渲染到对应位置上。

运行程序，正确进行了界面渲染则完成：

```
//全局区声明资源
#include "utils.h"

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

//SelectorScene私有成员变量
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

//编写on_enter方法初始化
this->m_animation_gamer1.setAtlas(&atlas_gamer1_die_right);
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
this->pos_img_1P_desc.x = (getwidth() - img_1P_desc.getwidth()) / 2 - OFFSET_X;
this->pos_img_1P_desc.y = getheight() - 150;
this->pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
this->pos_img_2P_desc.y = this->pos_img_1P_desc.y;
this->pos_img_1P_select_background.x = (getwidth() / 2 - img_select_background_right.getwidth()) / 2 - OFFSET_X;
this->pos_img_1P_select_background.y = this->pos_img_1P.y + img_1P.getwidth() + 35;
this->pos_img_1P_select_background.x = getwidth() / 2 + (getwidth() / 2 - img_select_background_left.getwidth()) / 2 + OFFSET_X;
this->pos_img_1P_select_background.y = this->pos_img_1P_select_background.y;
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

//编写on_draw方法渲染
putimage(0, 0, &img_selector_background);
putImageAlpha(this->pos_img_VS.x, this->pos_img_VS.y, &img_VS);

putImageAlpha(this->pos_img_1P.x, this->pos_img_1P.y, &img_1P);
putImageAlpha(this->pos_img_2P.x, this->pos_img_2P.y, &img_2P);
putImageAlpha(this->pos_img_1P_select_background.x, this->pos_img_1P_select_background.y, &img_select_background_right);
putImageAlpha(this->pos_img_2P_select_background.x, this->pos_img_2P_select_background.y, &img_select_background_left);

putImageAlpha(this->pos_img_1P_desc.x, this->pos_img_1P_desc.y, &img_1P_desc);
putImageAlpha(this->pos_img_2P_desc.x, this->pos_img_2P_desc.y, &img_2P_desc);

putImageAlpha(this->pos_img_tip.x, this->pos_img_tip.y, &img_selector_tip);
```



## 第六节代码完成展示

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

	void on_updata(int delta)
	{
		this->m_current_scene->on_update(delta);
	}

	void on_draw(const Camera& camera)
	{
		this->m_current_scene->on_draw(camera);
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

	virtual void on_enter() {}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera) {}

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

```

**scene.h**

```
#ifndef _SCENE_H_
#define _SCENE_H_

#include "camera.h"

#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {}
	virtual void on_update(int delta) {}
	virtual void on_draw(const Camera& camera) {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_exit() {}

private:

};

#endif
```

**selectorScene.h**

```
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
		this->pos_2P_selector_btn_left.x = this->pos_img_2P_select_background.x - img_2P_selector_btn_idle_left.getwidth();
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
```



## 第七节

### 角色选择界面渲染

为了区分选择角色类型，首先在selectorScene.h中的SelectorScene类内声明枚举类，并在类内声明角色1和2与Invalid类，随后可在类内声明成员变量指定玩家所选角色类型：

```
//类内私有成员枚举类
private:
	enum class PlayerType
	{
		Gamer1,
		Gamer2,
		Invalid
	};
	
//类内成员变量
PlayerType m_player_type_1 = PlayerType::Gamer1;
PlayerType m_player_type_2 = PlayerType::Gamer2;

```



接下来可以准备将画面渲染在界面上，不过在此之前需要对现有代码进行小幅度跳转，既然后续渲染需要先获取摄像机位置并且此处逻辑在所有渲染动画时都需要使用，那么结合面向对象的封装特性，不如直接将获取摄像机位置并与自身坐标进行做差的逻辑直接放入Animation类中。

首先在animation.h中引入camera.h头文件，随后将Camera引入on_draw参数并注意到putImageAlpha在其他函数也进行了引用，在utils.h中提供一个使用摄像机进行渲染的重载，随后在selectorScene.h中对on_draw方法中添加根据不同游戏角色选择不同动画逻辑进行渲染，只不过需要注意渲染顺序以免图片覆盖的问题，并且要记得在on_update中。

```
//animation.h头文件引入文件
#include "camera.h"
#include <graphics.h>

//utils.h头文件中重载带摄像头的putImageAlpha
inline void putImageAlpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - camera.getPosition().m_x), (int)(dst_y - camera.getPosition().m_y), w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//animation.h头文件中改写传入摄像机的渲染方法
void on_draw(const Camera& camera, int x, int y) const
{
	putImageAlpha(camera, x, y, this->m_atlas->getImage(this->m_idx_frame));
}

//selectorScene.h中改写on_draw渲染方法
virtual void on_draw(const Camera& camera)
{
	putimage(0, 0, &img_selector_background);
	putImageAlpha(this->pos_img_VS.x, this->pos_img_VS.y, &img_VS);

	putImageAlpha(this->pos_img_1P.x, this->pos_img_1P.y, &img_1P);
	putImageAlpha(this->pos_img_2P.x, this->pos_img_2P.y, &img_2P);
	putImageAlpha(this->pos_img_1P_select_background.x, this->pos_img_1P_select_background.y, &img_select_background_right);
	putImageAlpha(this->pos_img_2P_select_background.x, this->pos_img_2P_select_background.y, &img_select_background_left);

	switch (this->m_player_type_1)
	{
	case PlayerType::Gamer1:
		this->m_animation_gamer1.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
		break;
	case PlayerType::Gamer2:
		this->m_animation_gamer2.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
		break;
	}
	
	switch (this->m_player_type_2)
	{
	case PlayerType::Gamer1:
		this->m_animation_gamer1.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
		break;
	case PlayerType::Gamer2:
		this->m_animation_gamer2.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
		break;
	}

	putImageAlpha(this->pos_img_1P_desc.x, this->pos_img_1P_desc.y, &img_1P_desc);
	putImageAlpha(this->pos_img_2P_desc.x, this->pos_img_2P_desc.y, &img_2P_desc);

	putImageAlpha(this->pos_img_tip.x, this->pos_img_tip.y, &img_selector_tip);
}

//selectorScene.h中添加动画更新on_update方法
this->m_animation_gamer1.on_update(delta);
this->m_animation_gamer2.on_update(delta);
```



其次是角色文本角色名字符串图片，首先在私有成员中定义角色文本名字符串，接着在类内定义一个文本阴影效果文本的函数outtextxy_shaded其中outtextxy为EasyX提供的文本绘制方法，这里将文字颜色调低并移动3px使得字体带阴影呈现立体效果，最后是补全角色名绘图代码于on_draw的switch中，注意要先加载字体文件，于main.cpp文件中于main函数中于初始化窗口后设置字体样式为加载字体名同时设置文本背景色为透明：

```
//selectorScene.h中私有成员角色名变量
LPCTSTR str_gamer1_name = L" 角色 1 ";
LPCTSTR str_gamer2_name = L" 角色 2 ";

//SelectorScene私有成员函数
private:
void outtextxy_shaded(int x, int y, LPCTSTR str)
{
	settextcolor(RGB(45, 45, 45));
	outtextxy(x + 3, y + 3, str);
	settextcolor(RGB(255, 255, 255));
	outtextxy(x, y, str);
}

//selectorScene.h中修改on_draw方法中switch代码
switch (this->m_player_type_1)
{
case PlayerType::Gamer1:
	this->m_animation_gamer1.on_draw(camera, this->pos_animation_1P.x, this->pos_animation_1P.y);
	this->pos_img_1P_name.x = this->pos_img_1P_select_background.x + (img_select_background_right.getwidth() - textwidth(this->str_gamer1_name)) / 2;
	this->outtextxy_shaded(this->pos_img_1P_name.x, this->pos_img_1P_name.y, this->str_gamer1_name);
	break;
case PlayerType::Gamer2:
	this->m_animation_gamer2.on_draw(camera, this->pos_animation_2P.x, this->pos_animation_2P.y);
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

//main.cpp中添加字体
settextstyle(28, 0, L"HYPixel11pxU-2.ttf");
setbkmode(TRANSPARENT);
```



为了让画面加上更多动画效果让动画更加活跃，我们希望背景有一系列角色剪影水平滚动，且角色1背景剪影为角色2剪影，反之依然，且两图剪影都向界面中部进行滚动凸显中间的标志表现张力。

在素材包剪影中，直接提供了角色1&2的剪影背景图，也就是只需要这两张图片在画面上滚动即可，那么该如何让图片滚动起来？其中一种方法是渲染两次，假设存在一条扫描线，其左侧一张剪影右侧一张剪影只要扫描线从左侧到目的地再闪现到左侧，即可实现连续滚动特效。

首先selectorScene.h中选角场景类中私有成员中定义私有成员m_selector_background_scorll_offset_x背景板滚动距离，并在on_update中每次移动5px并当线条到达剪影宽度时就让线条重新回到原位，如此背景动态数据部分就完成了：

```
//selectorScene.h场景类私有成员变量
int m_selector_background_scorll_offset_x = 0;

//on_update更新方法变量移动
this->m_selector_background_scorll_offset_x += 5;
```



对于图片背景剪影，首先在SelectorScene类内on_draw开头声明两个图片指针分别指向角色1&2背后剪影图片对象，并根据type指向对应剪影，不过需要注意的是当线条移动时，一侧剪影不断变短另一侧不断变宽，也就是说需要对原始素材裁剪部分之后才能进行绘制，目前还无对图片进行裁剪的功能，所以在utils.h中为putImageAlpha再次提供一个可裁剪图片的重载版本，随后在selectorScene.h文件的场景类on_draw背景图绘制后添加剪影绘制方法，运行程序可以看到正常进行了渲染绘制：

```
//SelectorScene场景中on_draw方法增加指针与switch初始化方法
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

//utils.h文件提供存在裁剪putImageAlpha方法
inline void putImageAlpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width > 0 ? width : img->getwidth(), h = height > 0 ? height : img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//selectorScene.h中on_draw中绘制背景后添加
putImageAlpha(this->m_selector_background_scorll_offset_x - imgptr_P1_seletor_background->getwidth(), 0, imgptr_P1_seletor_background);
putImageAlpha(this->m_selector_background_scorll_offset_x, 0, imgptr_P1_seletor_background->getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P1_seletor_background, 0, 0);
putImageAlpha(getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P2_seletor_background);
putImageAlpha(getwidth() - imgptr_P2_seletor_background->getwidth(), 0, imgptr_P2_seletor_background->getwidth() - this->m_selector_background_scorll_offset_x, 0, imgptr_P2_seletor_background, this->m_selector_background_scorll_offset_x, 0);
```



### 角色选择界面交互

选角界面提供了选角按钮，但是与一般按钮不同，这些按钮不会提供响应操作仅作为视觉提示功能，当己方按下对应按键时便可以切换角色选择。

来到selecorScene.h文件类内声明四个布尔，分别标记1P与2P向左向右按钮是否按下，随后在on_input中处理事件，通过两层switch对按键按下时的不同码值处理，并且为了符合直觉将按键响应移至按键抬起时，并进行音效播放的效果，最后在on_draw方法中添加按钮绘制方法，左右两个两人共计四个按钮渲染，运行程序即可看到按钮变化与切换不同角色的渲染：

```
//selectorScene.h文件SelectorScene类内成员变量
bool is_btn_1P_left_down = false;
bool is_btn_1P_right_down = false;
bool is_btn_2P_left_down = false;
bool is_btn_2P_right_down = false;

//selectorScene.h文件on_input成员函数方法
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
		case VK_LEFT://'←'
			this->is_btn_2P_left_down = true;
			break;
		case VK_RIGHT://'→'
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
		case VK_LEFT://'←'
			this->is_btn_2P_left_down = false;
			this->m_player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)this->m_player_type_2 - 1) % (int)PlayerType::Invalid);
			mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
			break;
		case VK_RIGHT://'→'
			this->is_btn_2P_right_down = false;
			this->m_player_type_2 = (PlayerType)(((int)this->m_player_type_2 + 1) % (int)PlayerType::Invalid);
			mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
			break;
		}
		break;
	default:
		break;
	}
}

//selectorScene.h文件on_draw成员函数方法
putImageAlpha(this->pos_1P_selector_btn_left.x, this->pos_1P_selector_btn_left.y, this->is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
putImageAlpha(this->pos_1P_selector_btn_right.x, this->pos_1P_selector_btn_right.y, this->is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
putImageAlpha(this->pos_2P_selector_btn_left.x, this->pos_2P_selector_btn_left.y, this->is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
putImageAlpha(this->pos_2P_selector_btn_right.x, this->pos_2P_selector_btn_right.y, this->is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);
```



最后一个交互功能就是按下回车键后的功能，再次回到按键抬起时处理部分为switch添加回车条件分支，当回车抬起时通过场景管理器控制场景切换到游戏局内场景：

```
case VK_RETURN:
	scene_manager.switchTo(SceneManager::SceneType::Game);
	mciSendString(L"play ui_confirm from 0", NULL, 0, NULL);
	break;
```



## 第七节代码完成展示

**main.cpp**

```
#include "atlas.h"
#include "camera.h"

#include "scene.h"
#include "sceneManager.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"

#include "utils.h"

#pragma comment(lib, "Winmm.lib")

#include <graphics.h>

const int FPS = 60;

Camera main_camera;

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
```

**selectorScene.h**

```
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
			case VK_LEFT://'←'
				this->is_btn_2P_left_down = true;
				break;
			case VK_RIGHT://'→'
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
			case VK_LEFT://'←'
				this->is_btn_2P_left_down = false;
				this->m_player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)this->m_player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_RIGHT://'→'
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

	virtual void on_exit() {}

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

	LPCTSTR str_gamer1_name = L" 角色 1 ";
	LPCTSTR str_gamer2_name = L" 角色 2 ";

	int m_selector_background_scorll_offset_x = 0;

};

#endif
```

**animation.h**

```
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "atlas.h"
#include "utils.h"

#include "camera.h"
#include <graphics.h>
#include <functional>

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

	void on_draw(const Camera& camera, int x, int y) const
	{
		putImageAlpha(camera, x, y, this->m_atlas->getImage(this->m_idx_frame));
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

**utils.h**

```
#ifndef _UTILS_H_
#define _UTILS_H_

#pragma comment(lib, "MSIMG32.LIB")

#include <graphics.h>

inline void putImageAlpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putImageAlpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - camera.getPosition().m_x), (int)(dst_y - camera.getPosition().m_y), w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putImageAlpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width > 0 ? width : img->getwidth(), h = height > 0 ? height : img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
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



## 第八节

### 物理模拟引入

当我们在游戏中引入物理引擎的时候，牛顿就称为了整个游戏世界的导演。

游戏作品能否带给玩家以沉浸感很大程度取决于游戏中各类交互反馈是否符合玩家直觉，起跳后一段时间会落下，抛射物体会收重力落回地面，物理的模拟就是让世界的种种反馈尽可能区逼近显示存在。

### 局内场景界面绘制

首先来到gameScene.h文件中引入素材资源，并在GameScene中声明资源对应坐标，并在on_enter中初始化他们位置，由于为了摄像头抖动不显示黑边所以提供背景尺寸将会略大于窗口尺寸，并在on_draw中使用定义好的putImageAlpha方法渲染背景图片，运行程序正常看到背景即可：

```
//gameScene.h全局区引入资源
#include "utils.h"

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;

//GameScene成员变量声明
POINT pos_img_sky = { 0 };
POINT pos_img_hills = { 0 };

//GameScene成员函数on_enter
this->pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
this->pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

this->pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
this->pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

//GameScene成员函数on_draw
putImageAlpha(camera, this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
putImageAlpha(camera, this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);
```



### 简单物理实现——平台

接下来就可以实现简单的物理效果了，本项目较新的的碰撞效果"重力"效果，也就是在虚空内会受重力影响不断地受重力影响不断向下坠落，直到到达地面（或者摔死了>D<），也就是模拟重力的效果可以从悬浮空中的下坠和到达地面的停止，而关于地面也是相当抽象的概念。

在平台游戏中玩家可踩踏的对象，可以是游戏最下方的地面，也可以是箱子或道具，甚至是敌人或玩家，而在本项目中只考虑最简单的情况即玩家可落到"平台"上，所以我们可以将平台对象定义为类，但是我们也要思考应该使用什么样的几何图形来拟合平台碰撞区域，可能我们会第一时间想到矩形，但是要明确一点的是在游戏开发代码编写过程中，我们只需要让程序所展示出来的功能符合游戏逻辑即可，不必要的要求像显示一样对其进行一比一复刻。

所以也需要思考游戏平台是否有更好的设计思路，由于此类2D视角缺失了深度这一维度轴，所以对于此类游戏平台大多被设计为了单向碰撞，也就是玩家从下方可以正常穿过，从上方可以坠落到平台上，所以矩形碰撞箱就没有太大必要了，要关注的点实际上是平台对象的"地面"在哪，于是这种平台我们可以抽象成一条线。

所以代码方面，首先创建platform.h头文件定义Plantform类，在类内定义结构体CollisionShape表示碰撞外形，并在其中声明三个变量：高度，左侧点右侧点即可描述一条线最后在类内定义碰撞外形，添加IMAGE指针做渲染对象以及渲染对象坐标（碰撞位置与图片位置通常不一致，所以此处需要额外声明），引入绘图逻辑所需头文件，并声明on_draw方法使用camera在其中传递渲染出平台：

```
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "utils.h"
#include "camera.h"

class Platform
{
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putImageAlpha(this->m_render_position.x, this->m_render_position.y, this->m_img);
	}

public:
	struct CollisionShape
	{
		float y;
		float left, right;
	};

public:
	CollisionShape m_shape;

	IMAGE* m_img = nullptr;
	POINT m_render_position = { 0 };

};

#endif
```



而平台的存储此处还是使用全局数组vector存储，来到main.cpp文件引入所需头文件，在全局区添加vector平台类型向量，随后来到on_enter方法中添加初始化，由于目前只提供四个平台所以先初始化平台数量为4，所以以此给与其赋值操作，最后在on_draw中添加遍历绘制所有平台的方法，运行程序可以看到三个平台正常渲染则说明成功完成渲染部分：

```
//main.cpp中全局区添加代码
#include <vector>
#include "platform.h"

std::vector<Platform> platform_list;

//gameScene.h中全局区添加代码
#include "platform.h"

extern std::vector<Platform> platform_list;

//gameScene.h文件GameScene类内on_enter初始化
platform_list.resize(4);
Platform& large_platform = platform_list[0];
large_platform.m_img = &img_platform_large;
large_platform.m_render_position.x = 122;
large_platform.m_render_position.y = 455;
large_platform.m_shape.left = (float)large_platform.m_render_position.x + 30;
large_platform.m_shape.right = (float)large_platform.m_render_position.x + img_platform_large.getwidth() - 30;
large_platform.m_shape.y = (float)large_platform.m_render_position.y + 60;

Platform& small_platform_1 = platform_list[1];
small_platform_1.m_img = &img_platform_small;
small_platform_1.m_render_position.x = 175;
small_platform_1.m_render_position.y = 360;
small_platform_1.m_shape.left = (float)small_platform_1.m_render_position.x + 40;
small_platform_1.m_shape.right = (float)small_platform_1.m_render_position.x + img_platform_small.getwidth() - 40;
small_platform_1.m_shape.y = (float)small_platform_1.m_render_position.y + img_platform_small.getheight() / 2;

Platform& small_platform_2 = platform_list[2];
small_platform_2.m_img = &img_platform_small;
small_platform_2.m_render_position.x = 855;
small_platform_2.m_render_position.y = 360;
small_platform_2.m_shape.left = (float)small_platform_2.m_render_position.x + 40;
small_platform_2.m_shape.right = (float)small_platform_2.m_render_position.x + img_platform_small.getwidth() - 40;
small_platform_2.m_shape.y = (float)small_platform_2.m_render_position.y + img_platform_small.getheight() / 2;

Platform& small_platform_3 = platform_list[3];
small_platform_3.m_img = &img_platform_small;
small_platform_3.m_render_position.x = 515;
small_platform_3.m_render_position.y = 255;
small_platform_3.m_shape.left = (float)small_platform_3.m_render_position.x + 40;
small_platform_3.m_shape.right = (float)small_platform_3.m_render_position.x + img_platform_small.getwidth() - 40;
small_platform_3.m_shape.y = (float)small_platform_3.m_render_position.y + img_platform_small.getheight() / 2;

//gameScene.h文件GameScene类内on_enter初始化
for (const Platform& p : platform_list)
{
	p.on_draw(camera);
}

```



不过此处只有图形渲染效果不包含数据，所以我们希望添加简单的调试模式，只有就可以显示所有我们希望看见的外边框或数据，帮助开发者对这些抽象碰撞数据进行可视化检查。

首先来到main.cpp中定义is_debug布尔类型，随后在platform.h文件通过extern获取，并在Platform类内的on_draw方法中通过检查debug状态是否开启来绘制碰撞线，由于使用了Camera进行摄像机重载版本，所有在utils.h中重载一个line方法用于绘制碰撞线，最后在platform.h文件写on_draw方法绘制，并且在gameScene.h文件写on_input的切换debug方法，这时运行程序并按下Q键即可看到正确渲染碰撞横线，这种可视化调试对开发者编程工作帮助极大，后面在做玩家碰撞时便可以得见：

```
//main.cpp文件全局区
bool is_debug = false;

//platform.h文件全局区
extern bool is_debug;

//utils.h文件编写
inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.getPosition();
	line((int)(x1 - pos_camera.m_x), (int)(y1 - pos_camera.m_y), (int)(x2 - pos_camera.m_x), (int)(y2 - pos_camera.m_y));
}

//platform.h文件on_draw方法
if (is_debug)
{
	setlinecolor(RGB(255, 0, 0));
	line(camera, (int)this->m_shape.left, (int)this->m_shape.y, (int)this->m_shape.right, (int)this->m_shape.y);
}

//gameScene.h文件on_input方法
switch (msg.message)
{
case WM_KEYDOWN:
	break;
case WM_KEYUP:
	switch (msg.vkcode)
	{
	case 0x51://'Q'
		is_debug = !is_debug;
		break;
	default:
		break;
	}
	break;
default:
	break;
}
```



## 第八节完成代码展示

**main.cpp**

```
#include "atlas.h"
#include "camera.h"

#include "scene.h"
#include "sceneManager.h"
#include "menuScene.h"
#include "gameScene.h"
#include "selectorScene.h"

#include "utils.h"
#include "platform.h"

#pragma comment(lib, "Winmm.lib")

#include <graphics.h>
#include <vector>

const int FPS = 60;

bool is_debug = false;

Camera main_camera;

std::vector<Platform> platform_list;

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
```

**utils.h**

```
#ifndef _UTILS_H_
#define _UTILS_H_

#pragma comment(lib, "MSIMG32.LIB")

#include <graphics.h>

inline void putImageAlpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putImageAlpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - camera.getPosition().m_x), (int)(dst_y - camera.getPosition().m_y), w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putImageAlpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width > 0 ? width : img->getwidth(), h = height > 0 ? height : img->getheight();

	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.getPosition();
	line((int)(x1 - pos_camera.m_x), (int)(y1 - pos_camera.m_y), (int)(x2 - pos_camera.m_x), (int)(y2 - pos_camera.m_y));
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

**platform.h**

```
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "utils.h"
#include "camera.h"

extern bool is_debug;

class Platform
{
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putImageAlpha(this->m_render_position.x, this->m_render_position.y, this->m_img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)this->m_shape.left, (int)this->m_shape.y, (int)this->m_shape.right, (int)this->m_shape.y);
		}
	}

public:
	struct CollisionShape
	{
		float y;
		float left, right;
	};

public:
	CollisionShape m_shape;

	IMAGE* m_img = nullptr;
	POINT m_render_position = { 0 };

};

#endif
```

**gameScene.h**

```
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "utils.h"
#include "platform.h"
#include "scene.h"
#include "sceneManager.h"

#include <iostream>

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;

extern std::vector<Platform> platform_list;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void on_enter()
	{
		this->pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		this->pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		this->pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		this->pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform.m_img = &img_platform_large;
		large_platform.m_render_position.x = 122;
		large_platform.m_render_position.y = 455;
		large_platform.m_shape.left = (float)large_platform.m_render_position.x + 30;
		large_platform.m_shape.right = (float)large_platform.m_render_position.x + img_platform_large.getwidth() - 30;
		large_platform.m_shape.y = (float)large_platform.m_render_position.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.m_img = &img_platform_small;
		small_platform_1.m_render_position.x = 175;
		small_platform_1.m_render_position.y = 360;
		small_platform_1.m_shape.left = (float)small_platform_1.m_render_position.x + 40;
		small_platform_1.m_shape.right = (float)small_platform_1.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_1.m_shape.y = (float)small_platform_1.m_render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.m_img = &img_platform_small;
		small_platform_2.m_render_position.x = 855;
		small_platform_2.m_render_position.y = 360;
		small_platform_2.m_shape.left = (float)small_platform_2.m_render_position.x + 40;
		small_platform_2.m_shape.right = (float)small_platform_2.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_2.m_shape.y = (float)small_platform_2.m_render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.m_img = &img_platform_small;
		small_platform_3.m_render_position.x = 515;
		small_platform_3.m_render_position.y = 255;
		small_platform_3.m_shape.left = (float)small_platform_3.m_render_position.x + 40;
		small_platform_3.m_shape.right = (float)small_platform_3.m_render_position.x + img_platform_small.getwidth() - 40;
		small_platform_3.m_shape.y = (float)small_platform_3.m_render_position.y + img_platform_small.getheight() / 2;

	}

	virtual void on_update(int delta) {}

	virtual void on_draw(const Camera& camera) 
	{
		putImageAlpha(camera, this->pos_img_sky.x, this->pos_img_sky.y, &img_sky);
		putImageAlpha(camera, this->pos_img_hills.x, this->pos_img_hills.y, &img_hills);

		for (const Platform& p : platform_list)
		{
			p.on_draw(camera);
		}
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x51://'Q'
				is_debug = !is_debug;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	virtual void on_exit() {}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

};

#endif
```



## 第九节