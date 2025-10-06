#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <memory>

#include "ProtoWindow.h"
#include "TextBox.h"
#include "Surface.h"
#include "SuperTimer.h"

class MainWindow : public ProtoWindow {
private:
	std::unique_ptr<Surface> sprite;
	std::unique_ptr<SuperTimer> freeze_timer;
	std::unique_ptr<SuperTimer> anim_timer;
	std::unique_ptr<SuperTimer> cooldown_timer;
	int sprite_x;
	int sprite_y;
	int sprite_vx;
	int sprite_vy;

	DWORD sprite_stamp;
	void auto_sprite_move();
	void move_sprite_right();
	void move_sprite_left();
	void move_sprite_up();
	void move_sprite_down();
	void shift_sprite(int vx, int vy);
	void init_sprite();
	void delay_animation();

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