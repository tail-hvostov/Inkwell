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
	this->parent = parent;
}

ProtoControl::~ProtoControl() {}

void ProtoControl::query_client_rect(RECT* rect) {
	GetWindowRect(hwnd, rect);
	HWND parent_hwnd = parent->get_hwnd();

	POINT ptTopLeft = { rect->left, rect->top };
	POINT ptBottomRight = { rect->right, rect->bottom };

	ScreenToClient(parent_hwnd, &ptTopLeft);       // Левый верхний угол
	ScreenToClient(parent_hwnd, &ptBottomRight);   // Правый нижний угол

	rect->left = ptTopLeft.x;
	rect->top = ptTopLeft.y;
	rect->right = ptBottomRight.x;
	rect->bottom = ptBottomRight.y;
}