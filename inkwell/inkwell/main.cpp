#include <windows.h>

const char CLASS_NAME[] = "Inkwell Main Class";

//hInstance - ���������� ������.
//hPrevInstance - �����.
//pCmdLine - ��������� ��������� ������ � ���� ������ �������.
//nCmdShow - ����, �����������, �������� �� �������� ���� ����������
//��������, ���������� ��� ������������ � �������  ������.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PWSTR pCmdLine, int nCmdShow) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = NULL;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0, //����� ���� (��� �� ����� ������������)
		CLASS_NAME,
		"Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //X
		CW_USEDEFAULT, //Y
		CW_USEDEFAULT, //nWidth
		CW_USEDEFAULT, //nHeight
		NULL, //Parent
		NULL, //Menu
		hInstance,
		NULL //�������������� ������ �����������.
		);
	if (hwnd == NULL) {
		return 1;
	}
	return 0;
}