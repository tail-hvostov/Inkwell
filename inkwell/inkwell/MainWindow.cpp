#include "MainWindow.h"
#include "Application.h"
#include "resource.h"

#define TEXTBOX_MARGIN 10

namespace {
	const char CLASS_NAME[] = "Inkwell Main Class";
	const char MAIN_WINDOW_NAME[] = "Inkwell";
	bool is_class_registered = false;
};

void query_bitmap_dimensions(HBITMAP hbitmap, LONG* w, LONG* h) {
	if (hbitmap) {
		//—труктура содержит параметры BMP.
		BITMAP bm;
		GetObject(hbitmap, (int) sizeof bm, &bm);
		*w = bm.bmWidth;
		*h = bm.bmHeight;
	}
}

MainWindow* MainWindow::create() {
	if (!is_class_registered) {
		Application::register_window_class(CLASS_NAME, IDR_MENU1);
	}
	return new MainWindow();
}

MainWindow::MainWindow() : ProtoWindow(CLASS_NAME, MAIN_WINDOW_NAME,
										CW_USEDEFAULT, CW_USEDEFAULT)
{
	active_brush = CreateSolidBrush(RGB(0, 200, 60));
}

LRESULT MainWindow::on_paint(WPARAM wParam, LPARAM lParam) {
	//—труктура дл€ рисовани€ клиентской части окна.
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, active_brush);
	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT MainWindow::on_close(WPARAM wParam, LPARAM lParam) {
	if (MessageBox(hwnd, "¬ы точно хотите выйти?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
		DestroyWindow(hwnd);
	}
	return 0;
}

INT_PTR CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (WM_CLOSE == uMsg) {
		EndDialog(hwnd, NULL);
	}
	return FALSE;
}

LRESULT MainWindow::on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_COMMAND:
		{
			//я провер€ю, что сообщение послало меню.
			if (!HIWORD(wParam)) {
				switch (LOWORD(wParam)) {
				case ID_40003:
					set_active_color(120, 200, 60);
					invalidate_client();
					break;
				case ID_ABOUT:
					DialogBox(Application::Win32::get_hinstance(), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
					break;
				default:
					set_active_color(120, 0, 60);
					invalidate_client();
					break;
				}
			}
			return 0;
		}
	}
	return ProtoWindow::on_raw_msg(uMsg, wParam, lParam);
}

void MainWindow::set_active_color(BYTE r, BYTE g, BYTE b) {
	DeleteObject(active_brush);
	active_brush = CreateSolidBrush(RGB(r, g, b));
}

MainWindow::~MainWindow() {
	DeleteObject(active_brush);
}

void MainWindow::calc_textbox_rect(RECT* rect) {
	GetClientRect(hwnd, rect);
	rect->left = TEXTBOX_MARGIN;
	rect->right -= TEXTBOX_MARGIN;
	if (rect->right < rect->left) {
		rect->right = rect->left;
	}
	rect->top = TEXTBOX_MARGIN;
	rect->bottom -= TEXTBOX_MARGIN;
	if (rect->bottom < rect->top) {
		rect->bottom = rect->top;
	}
}

void MainWindow::on_create() {
	RECT rect;
	calc_textbox_rect(&rect);
	text_box.reset(new TextBox(rect.left, rect.top,
								rect.right - rect.left,
								rect.bottom - rect.top,
								this));
}

void MainWindow::on_resize(UINT width, UINT height) {
	text_box->set_size(width - TEXTBOX_MARGIN * 2, height - TEXTBOX_MARGIN * 2);
}