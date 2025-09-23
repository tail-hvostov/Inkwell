#include "ProtoControl.h"
#include "Application.h"

ProtoControl::ProtoControl(LPCSTR class_name, DWORD dwStyle, int x, int y, int w, int h, ControlParent* parent) {
	hwnd = CreateWindowEx(0,
						class_name,
						NULL,
						dwStyle,
						x,
						y,
						w,
						h,
						parent->get_hwnd(),
						NULL,
						Application::Win32::get_hinstance(),
						NULL);
}

ProtoControl::~ProtoControl() {}