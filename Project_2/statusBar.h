#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include "utils.h"
#include "vector2.h"

class StatusBar 
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void setAvatar(IMAGE* img)
	{
		this->m_img_avatar = img;
	}

	void setPosition(int x, int y)
	{
		this->m_position.m_x = x, this->m_position.m_y = y;
	}

	void setHp(int val)
	{
		this->m_hp = val;
	}

	void setMp(int val)
	{
		this->m_mp = val;
	}
	
	void on_draw()
	{
		putImageAlpha(this->m_position.m_x, this->m_position.m_y, this->m_img_avatar);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 10, this->m_position.m_x + 100 + this->width + 3 * 2, this->m_position.m_y + 36, 8, 8);
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 45, this->m_position.m_x + 100 + this->width + 3 * 2, this->m_position.m_y + 71, 8, 8);

		setfillcolor(RGB(67, 47, 47));
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 10, this->m_position.m_x + 100 + this->width + 3, this->m_position.m_y + 33, 8, 8);
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 45, this->m_position.m_x + 100 + this->width + 3, this->m_position.m_y + 68, 8, 8);

		float hp_bar_width = this->width * max(0, this->m_hp) / 100.0f;
		float mp_bar_width = this->width * min(100, this->m_mp) / 100.0f;

		setfillcolor(RGB(197, 61, 67));
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 10, this->m_position.m_x + 100 + (int)hp_bar_width + 3, this->m_position.m_y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(this->m_position.m_x + 100, this->m_position.m_y + 45, this->m_position.m_x + 100 + (int)mp_bar_width + 3, this->m_position.m_y + 68, 8, 8);
	}

private:
	const int width = 275;

private:
	int m_hp = 0;
	int m_mp = 0;

	Vector2 m_position = { 0, 0 };

	IMAGE* m_img_avatar = nullptr;

};

#endif