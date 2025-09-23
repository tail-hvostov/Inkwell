#ifndef _PROTO_WINDOW_H_
#define _PROTO_WINDOW_H_

#include <windows.h>

class ProtoWindow {
private:
	LRESULT window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND hwnd;

	ProtoWindow(const char* class_name, const char* title, int width, int height);

	virtual LRESULT on_paint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_close(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK foundation_window_proc(HWND hwnd, UINT uMsg,
													WPARAM wParam, LPARAM lParam);
	bool is_sufficient();
	void show(int nCmdShow);
	void invalidate_client();
	void query_client_dims(LONG* w, LONG* h);

	virtual ~ProtoWindow();
};

#endif