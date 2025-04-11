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