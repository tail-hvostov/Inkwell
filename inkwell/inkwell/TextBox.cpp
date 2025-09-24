#include "TextBox.h"

#define CLASS_NAME "EDIT"
#define STYLE WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL

TextBox::TextBox(int x, int y, int w, int h, ControlParent* parent) :
				ProtoControl(CLASS_NAME, STYLE, x, y, w, h, parent)  {}

void TextBox::set_size(int w, int h) {
	RECT rect;
	query_client_rect(&rect);
	SetWindowPos(hwnd, NULL, rect.left, rect.top, w, h, SWP_NOZORDER | SWP_NOOWNERZORDER);
}

char* TextBox::get_selected_text() {
	int length;
	char* result;

	DWORD dwStart, dwEnd;
	SendMessage(hwnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
	if (dwEnd > dwStart) {
		length = dwEnd - dwStart;
		result = new char[length + 1];
		int text_length = GetWindowTextLength(hwnd);
		char* buf = new char[text_length + 1];
		GetWindowText(hwnd, buf, text_length);
		strncpy(result, buf + dwStart, length);
		result[length] = 0;
		delete buf;
	}
	else {
		length = 1;
		result = new char[length];
		result[0] = 0;
	}

	return result;
}