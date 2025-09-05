#include "ProtoWindow.h"
#include "Application.h"

LRESULT CALLBACK ProtoWindow::foundation_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - код сообщени€.
	//ƒалее идут доп. параметры.
	switch (uMsg) {
	case WM_CREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			ProtoWindow* head = reinterpret_cast<ProtoWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)head);
			return 0;
		}
	case WM_DESTROY:
		{
			ProtoWindow* head = reinterpret_cast<ProtoWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			delete head;
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			ProtoWindow* head = reinterpret_cast<ProtoWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (head) {
				return head->window_proc(uMsg, wParam, lParam);
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

ProtoWindow::ProtoWindow(const char* class_name, const char* title, int width, int height) {
	hwnd = CreateWindowEx(
		0, //—тили окна (они не нужны потребител€м)
		class_name,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //X
		CW_USEDEFAULT, //Y
		width, //nWidth
		height, //nHeight
		NULL, //Parent
		NULL, //Menu
		Application::Win32::get_hinstance(),
		this //ƒополнительные данные приложени€.
		);
}

bool ProtoWindow::is_sufficient() {
	return hwnd != NULL;
}

void ProtoWindow::show(int nCmdShow) {
	if (hwnd) {
		ShowWindow(hwnd, nCmdShow);
	}
}

LRESULT ProtoWindow::window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_PAINT:
			return onPaint(wParam, lParam);
		case WM_CLOSE:
			return onClose(wParam, lParam);
		default:
			return onRawMsg(uMsg, wParam, lParam);
	}
}

LRESULT ProtoWindow::onPaint(WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, WM_PAINT, wParam, lParam);
}

LRESULT ProtoWindow::onClose(WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, WM_CLOSE, wParam, lParam);
}

LRESULT ProtoWindow::onRawMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

ProtoWindow::~ProtoWindow() {}