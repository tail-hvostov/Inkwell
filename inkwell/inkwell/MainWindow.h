#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "ProtoWindow.h"
#include "RTEntity.h"

enum PaintModes {
	Active,
	Passive
};

class MainWindow : public ProtoWindow, public RTEntity {
private:
	HBRUSH active_brush;
	HBRUSH passive_brush;
	HBITMAP sprite;
	LONG sprite_w;
	LONG sprite_h;
	UINT_PTR mode_timer;
	PaintModes paint_mode;

	HBRUSH get_bg_brush();
	void set_active_color(BYTE r, BYTE g, BYTE b);
	void set_active_mode();

	MainWindow();
protected:
	virtual LRESULT onPaint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT onClose(WPARAM wParam, LPARAM lParam);
	virtual LRESULT onRawMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static MainWindow* create();
	virtual void rt_update();

	void set_passive_mode();

	virtual ~MainWindow();
};

#endif