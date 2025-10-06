#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <functional>

#include "ProtoControl.h"

class TextBox : public ProtoControl {
private:
	bool is_visible;
	std::function<void()> on_change_listener;
protected:
	virtual void wnd_proc_payload(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	TextBox(int x, int y, int w, int h, ControlParent* parent);

	void set_size(int w, int h);
	char* get_selected_text();
	char* cut_selected_text();
	void paste_text(const char* text);
	char* get_text();
	void set_text(const char* text);
	void clear();
	void set_enabled(bool val);
	void set_visible(bool val);
	void set_on_change_listener(std::function<void()> listener);
};

#endif