#ifndef _CONTROL_PARENT_H_
#define _CONTROL_PARENT_H_

#include <Windows.h>

class ControlParent {
public:
	virtual HWND get_hwnd() = 0;
};

#endif