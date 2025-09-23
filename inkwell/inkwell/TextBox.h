#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "ProtoControl.h"

class TextBox : public ProtoControl {
public:
	TextBox(int x, int y, int w, int h, ControlParent* parent);
};

#endif