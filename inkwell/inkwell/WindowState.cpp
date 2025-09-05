#include "WindowState.h"
#include "resource.h"

VOID CALLBACK timer_callback(HWND hwnd, UINT unnamedParam2,
					UINT_PTR unnamedParam3, DWORD unnamedParam4) {
	WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	window_state->set_passive_mode();
}

void query_bitmap_dimensions(HBITMAP hbitmap, LONG* w, LONG* h) {
	if (hbitmap) {
		//—труктура содержит параметры BMP.
		BITMAP bm;
		GetObject(hbitmap, (int) sizeof bm, &bm);
		*w = bm.bmWidth;
		*h = bm.bmHeight;
	}
}

WindowState::WindowState(HWND hwnd, HINSTANCE hInstance) {
	passive_brush = CreateSolidBrush(RGB(0, 0, 0));
	active_brush = CreateSolidBrush(RGB(0, 200, 60));
	sprite = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPRITE));
	query_bitmap_dimensions(sprite, &sprite_w, &sprite_h);
	
	paint_mode = Active;
	this->hwnd = hwnd;
	this->hInstance = hInstance;
	timer = SetTimer(hwnd, NULL, 5000, timer_callback);
}

WindowState::~WindowState() {
	DeleteObject(passive_brush);
	DeleteObject(active_brush);
	DeleteObject(sprite);
	KillTimer(hwnd, timer);
}

void WindowState::set_active_color(BYTE r, BYTE g, BYTE b) {
	DeleteObject(active_brush);
	active_brush = CreateSolidBrush(RGB(r, g, b));
}

void WindowState::set_passive_mode() {
	InvalidateRect(hwnd, NULL, false);
	paint_mode = Passive;
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
		HDC hMemDC = CreateCompatibleDC(NULL);
		HBITMAP old_bitmap = (HBITMAP)SelectObject(hMemDC, sprite);
		
		BitBlt(hdc, 30, 60, sprite_w, sprite_h, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, old_bitmap);
		DeleteDC(hMemDC);
	}
	EndPaint(hwnd, &ps);
	return 0;
}