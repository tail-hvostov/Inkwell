#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <Windows.h>

class Surface {
private:
	HBITMAP bitmap;
public:
	Surface(int resource);
	~Surface();
};

#endif