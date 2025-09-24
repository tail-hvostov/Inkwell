#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <memory>

#include "ProtoWindow.h"
#include "TextBox.h"

class MainWindow : public ProtoWindow {
private:
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
protected:
	virtual void on_create();
	virtual LRESULT on_paint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual void on_menu_press(WORD item);
	virtual void on_resize(UINT width, UINT height);
	virtual void on_control_notification(ProtoControl* control, WORD notification);
public:
	static MainWindow* create();
};

#endif