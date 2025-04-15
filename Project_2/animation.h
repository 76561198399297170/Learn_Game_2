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