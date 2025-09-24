#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <memory>

#include "ProtoWindow.h"
#include "TextBox.h"

class MainWindow : public ProtoWindow {
private:
	std::unique_ptr<TextBox> text_box;

	MainWindow();
	void calc_textbox_rect(RECT* rect);

	void on_copy();
protected:
	virtual void on_create();
	virtual LRESULT on_paint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual void on_menu_press(WORD item);
	virtual void on_resize(UINT width, UINT height);
public:
	static MainWindow* create();
};

#endif