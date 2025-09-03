#include <windows.h>

const char CLASS_NAME[] = "Inkwell Main Class";
const char MAIN_WINDOW_NAME[] = "Inkwell";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//uMsg - ��� ���������.
	//����� ���� ���. ���������.
	switch (uMsg) {
	case WM_PAINT:
		{
			//��������� ��� ��������� ���������� ����� ����.
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW));
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, "��� ��� �����?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
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

//hInstance - ���������� ������.
//hPrevInstance - �����.
//pCmdLine - ��������� ��������� ������ � ���� ������ �������.
//nCmdShow - ����, �����������, �������� �� �������� ���� ����������
//��������, ���������� ��� ������������ � �������  ������.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PWSTR pCmdLine, int nCmdShow) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
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