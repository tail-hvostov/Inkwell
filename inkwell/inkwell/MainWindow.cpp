#include "MainWindow.h"
#include "Application.h"
#include "resource.h"

#define TEXTBOX_MARGIN 10
#define BG_COLOR RGB(0, 200, 60)

namespace {
	const char CLASS_NAME[] = "Inkwell Main Class";
	const char MAIN_WINDOW_NAME[] = "Inkwell";
	bool is_class_registered = false;
};

void query_bitmap_dimensions(HBITMAP hbitmap, LONG* w, LONG* h) {
	if (hbitmap) {
		//Структура содержит параметры BMP.
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
{}

LRESULT MainWindow::on_paint(WPARAM wParam, LPARAM lParam) {
	//Структура для рисования клиентской части окна.
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HBRUSH brush = CreateSolidBrush(BG_COLOR);
	FillRect(hdc, &ps.rcPaint, brush);
	DeleteObject(brush);
	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT MainWindow::on_close(WPARAM wParam, LPARAM lParam) {
	if (MessageBox(hwnd, "Вы точно хотите выйти?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
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

void MainWindow::on_menu_press(WORD item) {
	switch (item) {
	case ID_MCOPY:
		show_message("copy");
		break;
	case ID_MQUIT:
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case ID_MABOUT:
		DialogBox(Application::Win32::get_hinstance(), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
		break;
	default:
		show_message("This operation has not been implemented.");
		break;
	}
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