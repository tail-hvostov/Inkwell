#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "ProtoWindow.h"

enum PaintModes {
	Active,
	Passive
};

class MainWindow : public ProtoWindow {
private:
	HBRUSH active_brush;
	HBRUSH passive_brush;
	HBITMAP sprite;
	LONG sprite_w;
	LONG sprite_h;
	UINT_PTR mode_timer;
	UINT_PTR anim_timer;
	PaintModes paint_mode;
	LONG sprite_x;
	LONG sprite_y;
	LONG sprite_vx;
	LONG sprite_vy;
	DWORD sprite_stamp;

	HBRUSH get_bg_brush();
	void set_active_color(BYTE r, BYTE g, BYTE b);
	void set_active_mode();
	void update_sprite_pos();

	MainWindow();
	void init_sprite();
protected:
	virtual LRESULT onPaint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT onClose(WPARAM wParam, LPARAM lParam);
	virtual LRESULT onRawMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static MainWindow* create();

	void set_passive_mode();

	virtual ~MainWindow();
};

#endif