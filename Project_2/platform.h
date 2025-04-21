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