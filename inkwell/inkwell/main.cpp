#include "resource.h"
#include "WindowState.h"

#include <windows.h>

const char CLASS_NAME[] = "Inkwell Main Class";
const char MAIN_WINDOW_NAME[] = "Inkwell";

HINSTANCE hInst;

INT_PTR CALLBACK AboutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (WM_CLOSE == uMsg) {
		EndDialog(hwnd, NULL);
	}
	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - ��� ���������.
	//����� ���� ���. ���������.
	switch (uMsg) {
	case WM_PAINT:
		{
			WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			return window_state->paint();
		}
	case WM_CLOSE:
		if (MessageBox(hwnd, "��� ��� �����?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
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
			WindowState* window_state = new WindowState(hwnd);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window_state);
		}
		return 0;
	case WM_COMMAND:
		{
			//� ��������, ��� ��������� ������� ����.
			if (!HIWORD(wParam)) {
				WindowState* window_state = (WindowState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				switch (LOWORD(wParam)) {
				case ID_40003:
					window_state->set_active_color(120, 200, 60);
					InvalidateRect(hwnd, NULL, false);
					break;
				case ID_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
					break;
				default:
					window_state->set_active_color(120, 0, 60);
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

//hInstance - ���������� ������.
//hPrevInstance - �����.
//pCmdLine - ��������� ��������� ������ � ���� ������ �������.
//nCmdShow - ����, �����������, �������� �� �������� ���� ����������
//��������, ���������� ��� ������������ � �������  ������.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PWSTR pCmdLine, int nCmdShow) {
	hInst = hInstance;

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0, //����� ���� (��� �� ����� ������������)
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
		NULL //�������������� ������ ����������.
		);
	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	//GetMessage ��������� ��������� ��� ������ ����, 
	//������� ����������� ����������� ������ � ��������� ������, 
	//���������� � ������� ����������� ������ ��� ������
	//������������� ������� PostThreadMessage.
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		//��������� ������� ������ � �������.
		TranslateMessage(&msg);
		//������� ��������� � ��������� ����.
		DispatchMessage(&msg);
	}
	return 0;
}