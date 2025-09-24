#ifndef _PROTO_WINDOW_H_
#define _PROTO_WINDOW_H_

#include <windows.h>

#include "ControlParent.h"

class ProtoWindow : virtual public ControlParent {
private:
	LRESULT window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND hwnd;

	ProtoWindow(const char* class_name, const char* title, int width, int height);

	void show_message(const char* message);

	virtual void on_create();
	virtual LRESULT on_paint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual void on_resize(UINT width, UINT height);
	virtual void on_menu_press(WORD item);
	virtual LRESULT on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK foundation_window_proc(HWND hwnd, UINT uMsg,
													WPARAM wParam, LPARAM lParam);
	bool is_sufficient();
	void show(int nCmdShow);
	void invalidate_client();
	void query_client_dims(LONG* w, LONG* h);

	virtual HWND get_hwnd();

	virtual ~ProtoWindow();
};

#endif