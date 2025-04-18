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
	int m_pass_time = 0;//�ѹ�ʱ��
	int m_wait_time = 0;//�ȴ�ʱ��
	bool is_paused = false;//�Ƿ���ͣ
	bool is_shotted = false;//�Ƿ񴥷�
	bool is_one_shot = false;//�Ƿ񵥴δ���
	std::function<void()> m_callback;//�ص�����

};

#endif