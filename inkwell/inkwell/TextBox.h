#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "ProtoControl.h"

class TextBox : public ProtoControl {
public:
	TextBox(int x, int y, int w, int h, ControlParent* parent);

	void set_size(int w, int h);
	char* get_selected_text();
	char* cut_selected_text();
	void paste_text(const char* text);
	char* get_text();
	void clear();
};

#endif