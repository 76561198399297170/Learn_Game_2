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