#include <windows.h>

#include "resource.h"

const char CLASS_NAME[] = "Inkwell Main Class";
const char MAIN_WINDOW_NAME[] = "Inkwell";

HINSTANCE hInst;

struct WindowState {
	HBRUSH color;
};

INT_PTR CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (WM_CLOSE == uMsg) {
		EndDialog(hwnd, NULL);
	}
	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - код сообщения.
	//Далее идут доп. параметры.
	switch (uMsg) {
	case WM_PAINT:
		{
			WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			//Структура для рисования клиентской части окна.
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, window_state->color);
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, "Лох или пидор?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		{
			WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			delete window_state;
			PostQuitMessage(0);
		}
		return 0;
	case WM_CREATE:
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			WindowState* window_state = reinterpret_cast<WindowState*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window_state);
		}
		return 0;
	case WM_COMMAND:
		{
			//Я проверяю, что сообщение послало меню.
			if (!HIWORD(wParam)) {
				WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				switch (LOWORD(wParam)) {
				case ID_40003:
					window_state->color = (HBRUSH)COLOR_HIGHLIGHTTEXT;
					InvalidateRect(hwnd, NULL, false);
					break;
				case ID_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
					break;
				default:
					window_state->color = (HBRUSH)COLOR_WINDOW + 1;
					InvalidateRect(hwnd, NULL, false);
					break;
				}
			}
		}
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

//hInstance - дескриптор модуля.
//hPrevInstance - мусор.
//pCmdLine - аргументы командной строки в виде строки Юникода.
//nCmdShow - флаг, указывающий, является ли основное окно приложения
//свёрнутым, развёрнутым или отображаемым в обычном  режиме.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PWSTR pCmdLine, int nCmdShow) {
	hInst = hInstance;

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&wc);

	WindowState* window_state = new WindowState();
	window_state->color = (HBRUSH)COLOR_GRAYTEXT;

	HWND hwnd = CreateWindowEx(
		0, //Стили окна (они не нужны потребителям)
		CLASS_NAME,
		MAIN_WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //X
		CW_USEDEFAULT, //Y
		CW_USEDEFAULT, //nWidth
		CW_USEDEFAULT, //nHeight
		NULL, //Parent
		NULL, //Menu
		hInstance,
		window_state //Дополнительные данные приложения.
		);
	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	//GetMessage извлекает сообщения для любого окна, 
	//которое принадлежит вызывающему потоку и сообщения потока, 
	//помещенные в очередь вызывающего потока при помощи
	//использования функции PostThreadMessage.
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		//Переводит нажатия клавиш в символы.
		TranslateMessage(&msg);
		//Передаёт сообщение в процедуру окна.
		DispatchMessage(&msg);
	}
	return 0;
}