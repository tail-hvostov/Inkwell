#include "WindowState.h"

VOID CALLBACK timer_callback(HWND hwnd, UINT unnamedParam2,
					UINT_PTR unnamedParam3, DWORD unnamedParam4) {
	WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	window_state->set_active_color(0, 0, 0);
	window_state->set_passive_mode();
}

WindowState::WindowState(HWND hwnd) {
	passive_brush = CreateSolidBrush(RGB(0, 0, 0));
	active_brush = CreateSolidBrush(RGB(0, 200, 60));
	paint_mode = Active;
	this->hwnd = hwnd;
	timer = SetTimer(hwnd, NULL, 5000, timer_callback);
}

WindowState::~WindowState() {
	DeleteObject(passive_brush);
	DeleteObject(active_brush);
	KillTimer(hwnd, timer);
}

void WindowState::set_active_color(BYTE r, BYTE g, BYTE b) {
	DeleteObject(active_brush);
	active_brush = CreateSolidBrush(RGB(r, g, b));
}

void WindowState::set_passive_mode() {
	InvalidateRect(hwnd, NULL, false);
}

HBRUSH WindowState::get_bg_brush() {
	return (Active == paint_mode) ? active_brush : passive_brush;
}

int WindowState::paint() {
	//—труктура дл€ рисовани€ клиентской части окна.
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, get_bg_brush());
	if (Passive == paint_mode) {
		
	}
	EndPaint(hwnd, &ps);
	return 0;
}