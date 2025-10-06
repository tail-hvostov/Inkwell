#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <Windows.h>

class Surface {
private:
	HBITMAP bitmap;
	LONG w;
	LONG h;
public:
	Surface(int resource);
	void blit(HDC hdc, int x, int y);
	void query_dimensions(LONG* w, LONG* h);
	LONG get_w();
	LONG get_h();
	~Surface();
};

#endif