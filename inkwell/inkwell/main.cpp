#include <windows.h>

const char CLASS_NAME[] = "Inkwell Main Class";
const char MAIN_WINDOW_NAME[] = "Inkwell";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - код сообщения.
	//Далее идут доп. параметры.
	switch (uMsg) {
	case WM_PAINT:
		{
			//Структура для рисования клиентской части окна.
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW));
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, "Лох или пидор?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
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
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

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
		NULL //Дополнительные данные приложения.
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