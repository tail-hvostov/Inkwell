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
		GetWindowText(hwnd, buf, text_length + 1);
		memcpy(result, buf + dwStart, length);
		result[length] = 0;
		delete[] buf;
	}
	else {
		length = 1;
		result = new char[length];
		result[0] = 0;
	}

	return result;
}

char* TextBox::cut_selected_text() {
	int length;
	char* result;

	DWORD dwStart, dwEnd;
	SendMessage(hwnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
	if (dwEnd > dwStart) {
		length = dwEnd - dwStart;
		result = new char[length + 1];
		int text_length = GetWindowTextLength(hwnd);
		char* buf = new char[text_length + 1];
		GetWindowText(hwnd, buf, text_length + 1);
		memcpy(result, buf + dwStart, length);
		result[length] = 0;

		char* tail = buf + dwEnd;
		char* newTail = buf + dwStart;
		while (*tail != 0) {
			*newTail = *tail;
			tail++;
			newTail++;
		}
		*newTail = 0;
		SetWindowText(hwnd, buf);
		delete[] buf;
	}
	else {
		length = 1;
		result = new char[length];
		result[0] = 0;
	}

	return result;
}

void TextBox::paste_text(const char* text) {
	int text_len = strlen(text);
	int parag_len = GetWindowTextLength(hwnd);
	char* buf = new char[text_len + parag_len + 1];
	GetWindowText(hwnd, buf, parag_len + 1);
	DWORD dwStart;
	SendMessage(hwnd, EM_GETSEL, (WPARAM)&dwStart, NULL);

	char* tail = buf + parag_len;
	char* new_tail = buf + parag_len + text_len;
	while (tail >= buf + dwStart) {
		*new_tail = *tail;
		tail--;
		new_tail--;
	}
	memcpy(buf + dwStart, text, text_len);

	SetWindowText(hwnd, buf);
	SendMessage(hwnd, EM_SETSEL, dwStart + text_len, dwStart + text_len);
	delete[] buf;
}

char* TextBox::get_text() {
	int text_length = GetWindowTextLength(hwnd);
	char* buf = new char[text_length + 1];
	GetWindowText(hwnd, buf, text_length + 1);
	return buf;
}

void TextBox::clear() {
	SetWindowText(hwnd, "");
}

void TextBox::set_text(const char* text) {
	SetWindowText(hwnd, text);
}