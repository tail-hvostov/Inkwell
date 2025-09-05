#ifndef _WINDOW_STATE_H_
#define _WINDOW_STATE_H_

#include <windows.h>

enum PaintModes {
	Active,
	Passive
};

struct WindowState {
private:
	HBRUSH active_brush;
	HBRUSH passive_brush;
	HBITMAP sprite;
	LONG sprite_w;
	LONG sprite_h;

	HINSTANCE hInstance;

	UINT_PTR timer;
	HWND hwnd;
	PaintModes paint_mode;

	HBRUSH get_bg_brush();
public:
	WindowState(HWND hwnd, HINSTANCE hInstance);
	~WindowState();

	void set_active_color(BYTE r, BYTE g, BYTE b);
	void set_passive_mode();

	int paint();
};

#endif // _WINDOW_STATE_H_