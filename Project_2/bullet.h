#ifndef _BULLET_H_
#define _BULLET_H_

#include "vector2.h"
#include "playerId.h"
#include "camera.h"

#include <functional>
#include <graphics.h>

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	virtual void on_collide()
	{
		if (this->m_callback) this->m_callback();
	}

	virtual bool checkCollide(const Vector2& position, const Vector2& size)
	{
		return this->m_position.m_x + this->m_size.m_x / 2 >= position.m_x &&
			this->m_position.m_x + this->m_size.m_x / 2 <= position.m_x + size.m_x && 
			this->m_position.m_y + this->m_size.m_y / 2 >= position.m_y && 
			this->m_position.m_y + this->m_size.m_y / 2 <= position.m_y + size.m_y;
	}

	virtual void on_update(int delta)
	{
	}

	virtual void on_draw(const Camera& camera) const
	{
		if (is_debug)
		{
			setfillcolor((RGB(255, 255, 255)));
			setlinecolor((RGB(255, 255, 255)));

			rectangle((int)this->m_position.m_x, (int)this->m_position.m_y, (int)this->m_position.m_x + this->m_size.m_x, (int)this->m_position.m_y + this->m_size.m_y);
			solidcircle((int)this->m_position.m_x + this->m_size.m_x / 2, (int)this->m_position.m_y + this->m_size.m_y / 2, 5);
		}
	}

	int getDamage() { return this->m_damage; }

	const Vector2& getPosition() { return this->m_position; }

	const Vector2& getSize() { return this->m_size; }

	PlayerId getCollideTarget() const { return this->m_target_id; }

	bool getValid() { return this->is_valid; }

	bool checkCanRemove() const { return this->can_remove; }

	void setCallback(std::function<void()> callback) { this->m_callback = callback; }

	void setDamage(int damage) { this->m_damage = damage; }

	void setPosition(float x, float y) { this->m_position.m_x = x, this->m_position.m_y = y; }

	void setVelocity(float x, float y) { this->m_velocity.m_x = x, this->m_velocity.m_y = y; }

	void setCollideTarget(PlayerId target) { this->m_target_id = target; }

	void setValid(bool is_valid) { this->is_valid = is_valid; }

protected:
	bool checkIfExceedScreen()
	{
		return (this->m_position.m_x + this->m_size.m_x <= 0 || this->m_position.m_x > getwidth() || 
			this->m_position.m_y + this->m_size.m_y <= 0 || this->m_position.m_y > getheight());
	}

protected:
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;

	int m_damage = 10;
	
	bool is_valid = true;
	bool can_remove = false;

	std::function<void()> m_callback;

	PlayerId m_target_id = PlayerId::P1;

};

#endif