#include "ProtoControl.h"
#include "Application.h"

LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ProtoControl* control = ProtoControl::get_control_from_hwnd(hwnd);
	return control->control_wnd_proc(uMsg, wParam, lParam);
}

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
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	original_proc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);
}

ProtoControl* ProtoControl::get_control_from_hwnd(HWND hwnd) {
	return (ProtoControl*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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

void ProtoControl::wnd_proc_payload(UINT uMsg, WPARAM wParam, LPARAM lParam) {}

LRESULT ProtoControl::control_wnd_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	wnd_proc_payload(uMsg, wParam, lParam);
	return CallWindowProc(original_proc, hwnd, uMsg, wParam, lParam);
}