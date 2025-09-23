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
	void set_active_color(BYTE r, BYTE g, BYTE b);

	MainWindow();
protected:
	virtual LRESULT on_paint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static MainWindow* create();

	virtual ~MainWindow();
};

#endif