#ifndef _PROTO_CONTROL_H_
#define _PROTO_CONTROL_H_

#include <Windows.h>

#include "ControlParent.h"

class ProtoControl {
protected:
	HWND hwnd;

	ProtoControl(LPCSTR class_name, DWORD dwStyle, int x, int y,
					int w, int h, ControlParent* parent);
public:
	virtual ~ProtoControl();
};

#endif