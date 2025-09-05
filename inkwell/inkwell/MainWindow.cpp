#include "MainWindow.h"
#include "Application.h"
#include "resource.h"

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
	passive_brush = CreateSolidBrush(RGB(0, 0, 0));
	active_brush = CreateSolidBrush(RGB(0, 200, 60));
	sprite = LoadBitmap(Application::Win32::get_hinstance(), MAKEINTRESOURCE(IDB_SPRITE));
	query_bitmap_dimensions(sprite, &sprite_w, &sprite_h);
	set_active_mode();
}

LRESULT MainWindow::onPaint(WPARAM wParam, LPARAM lParam) {
	//—труктура дл€ рисовани€ клиентской части окна.
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, get_bg_brush());
	if (Passive == paint_mode) {
		HDC hMemDC = CreateCompatibleDC(NULL);
		HBITMAP old_bitmap = (HBITMAP)SelectObject(hMemDC, sprite);
		
		BitBlt(hdc, 30, 60, sprite_w, sprite_h, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, old_bitmap);
		DeleteDC(hMemDC);
	}
	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT MainWindow::onClose(WPARAM wParam, LPARAM lParam) {
	if (MessageBox(hwnd, "Ћох или пидор?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
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

LRESULT MainWindow::onRawMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_COMMAND:
		{
			//я провер€ю, что сообщение послало меню.
			if (!HIWORD(wParam)) {
				switch (LOWORD(wParam)) {
				case ID_40003:
					set_active_color(120, 200, 60);
					InvalidateRect(hwnd, NULL, false);
					break;
				case ID_ABOUT:
					DialogBox(Application::Win32::get_hinstance(), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
					break;
				default:
					set_active_color(120, 0, 60);
					InvalidateRect(hwnd, NULL, false);
					break;
				}
			}
		}
		return 0;
	}
	return ProtoWindow::onRawMsg(uMsg, wParam, lParam);
}

void MainWindow::rt_update() {
}

HBRUSH MainWindow::get_bg_brush() {
	return (Active == paint_mode) ? active_brush : passive_brush;
}

void MainWindow::set_active_color(BYTE r, BYTE g, BYTE b) {
	DeleteObject(active_brush);
	active_brush = CreateSolidBrush(RGB(r, g, b));
}

VOID CALLBACK timer_callback(HWND hwnd, UINT unnamedParam2,
					UINT_PTR unnamedParam3, DWORD unnamedParam4) {
	MainWindow* head = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	head->set_passive_mode();
}

void MainWindow::set_active_mode() {
	paint_mode = Active;
	mode_timer = SetTimer(hwnd, NULL, 5000, timer_callback);
}

void MainWindow::set_passive_mode() {
	KillTimer(hwnd, mode_timer);
	mode_timer = NULL;
	InvalidateRect(hwnd, NULL, false);
	paint_mode = Passive;
}

MainWindow::~MainWindow() {
	DeleteObject(passive_brush);
	DeleteObject(active_brush);
	DeleteObject(sprite);
	if (mode_timer) {
		KillTimer(hwnd, mode_timer);
	}
}