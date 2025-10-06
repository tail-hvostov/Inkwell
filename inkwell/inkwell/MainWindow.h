#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <memory>

#include "ProtoWindow.h"
#include "TextBox.h"
#include "Surface.h"

class MainWindow : public ProtoWindow {
private:
	std::unique_ptr<Surface> sprite;

	bool unsaved_changes;
	std::unique_ptr<TextBox> text_box;

	MainWindow();
	void calc_textbox_rect(RECT* rect);

	void on_copy();
	void on_cut();
	void on_place();
	bool on_save();
	void on_new_document();
	void on_open();

	void load_text(const char* file_name);
	bool save_text(const char* file_name);
	bool unsaved_changes_prompt();

	void shift_mode();
	bool sprite_mode;
protected:
	virtual void on_create();
	virtual void on_paint(PAINTSTRUCT* ps);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual void on_menu_press(WORD item);
	virtual void on_resize(UINT width, UINT height);
	virtual void on_control_notification(ProtoControl* control, WORD notification);
	virtual void on_keydown(WPARAM key, LPARAM params);
	virtual LRESULT on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static MainWindow* create();
};

#endif