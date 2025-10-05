#include "ProtoWindow.h"
#include "Application.h"
#include "ProtoControl.h"

#define WM_DCREATE WM_APP

LRESULT CALLBACK ProtoWindow::foundation_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - ��� ���������.
	//����� ���� ���. ���������.
	switch (uMsg) {
	case WM_CREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			ProtoWindow* head = reinterpret_cast<ProtoWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)head);
			PostMessage(hwnd, WM_DCREATE, 0, 0);
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
		0, //����� ���� (��� �� ����� ������������)
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
		this //�������������� ������ ����������.
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
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			LRESULT result = on_paint(&ps);
			EndPaint(hwnd, &ps);
			return result;
		}
		case WM_CLOSE:
			return on_close(wParam, lParam);
		case WM_DCREATE:
			on_create();
			return 0;
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
			on_resize(width, height);
			return 0;
		}
		case WM_COMMAND:
		{
			//� ��������, ��� ��������� ������� ����.
			if (!HIWORD(wParam)) {
				on_menu_press(LOWORD(wParam));
				
			}
			//��� ������� ����������.
			else if (lParam) {
				ProtoControl* control = ProtoControl::get_control_from_hwnd((HWND)lParam);
				if (control) {
					on_control_notification(control, HIWORD(wParam));
				}
			}
			return 0;
		}
		default:
			return on_raw_msg(uMsg, wParam, lParam);
	}
}

LRESULT ProtoWindow::on_paint(PAINTSTRUCT* ps) {
	return DefWindowProc(hwnd, WM_PAINT, 0, 0);
}

LRESULT ProtoWindow::on_close(WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, WM_CLOSE, wParam, lParam);
}

LRESULT ProtoWindow::on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ProtoWindow::on_resize(UINT width, UINT height) {}

void ProtoWindow::on_create() {}

ProtoWindow::~ProtoWindow() {}

void ProtoWindow::invalidate_client() {
	InvalidateRect(hwnd, NULL, false);
}

void ProtoWindow::query_client_dims(LONG* w, LONG* h) {
	RECT rect;
	if (GetClientRect(hwnd, &rect)) {
		*w = rect.right;
		*h = rect.bottom;
	}
}

HWND ProtoWindow::get_hwnd() {
	return hwnd;
}

void ProtoWindow::on_menu_press(WORD item) {}

void ProtoWindow::show_message(const char* message) {
	MessageBox(NULL, message, "Alert", MB_OK);
}

void ProtoWindow::on_control_notification(ProtoControl* control, WORD notification) {}