#ifndef _PROTO_CONTROL_H_
#define _PROTO_CONTROL_H_

#include <Windows.h>

#include "ControlParent.h"

class ProtoControl {
private:
	WNDPROC original_proc;
protected:
	HWND hwnd;
	ControlParent* parent;

	ProtoControl(LPCSTR class_name, DWORD dwStyle, int x, int y,
					int w, int h, ControlParent* parent);

	void query_client_rect(RECT* rect);
	virtual void wnd_proc_payload(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	virtual ~ProtoControl();

	LRESULT control_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static ProtoControl* get_control_from_hwnd(HWND hwnd);
};

#endif