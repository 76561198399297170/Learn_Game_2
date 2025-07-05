#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "utils.h"
#include "atlas.h"
#include "vector2.h"
#include "camera.h"

class Particle
{
public:
	Particle() = default;

	Particle(const Vector2& position, Atlas* atlas, int lifespan) : m_position(position), m_atlas(atlas), m_lifespan(lifespan) {}

	~Particle() = default;

	void on_update(int delta)
	{
		this->m_timer += delta;
		if (this->m_timer >= this->m_lifespan)
		{
			this->m_timer = 0;
			this->m_idx_frame++;
			
			if (this->m_idx_frame >= this->m_atlas->getSize()) 
			{
				this->m_idx_frame = this->m_atlas->getSize() - 1;
				this->is_valid = false;
			}
		}
	}

	void on_draw(const Camera& camera) const 
	{
		putImageAlpha(camera, (int)this->m_position.m_x, (int)this->m_position.m_y, this->m_atlas->getImage(this->m_idx_frame));
	}

	void setAtlas(Atlas* atlas) { this->m_atlas = atlas; }

	void setPosition(const Vector2& position) { this->m_position = position; }

	void setLifespan(int ms) { this->m_lifespan = ms; }

	bool checkValid() const { return this->is_valid; }

private:
	int m_timer = 0;
	int m_lifespan = 0;
	int m_idx_frame = 0;

	bool is_valid = true;

	Vector2 m_position;

	Atlas* m_atlas = nullptr;

};

#endif