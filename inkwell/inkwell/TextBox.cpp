#include "TextBox.h"

#define CLASS_NAME "EDIT"
#define STYLE WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL

TextBox::TextBox(int x, int y, int w, int h, ControlParent* parent) :
				ProtoControl(CLASS_NAME, STYLE, x, y, w, h, parent)  {}