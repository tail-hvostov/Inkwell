#ifndef _PROTO_CONTROL_H_
#define _PROTO_CONTROL_H_

#include <Windows.h>

#include "ControlParent.h"

class ProtoControl {
protected:
	HWND hwnd;
	ControlParent* parent;

	ProtoControl(LPCSTR class_name, DWORD dwStyle, int x, int y,
					int w, int h, ControlParent* parent);

	void query_client_rect(RECT* rect);
public:
	virtual ~ProtoControl();

	static ProtoControl* get_control_from_hwnd(HWND hwnd);
};

#endif