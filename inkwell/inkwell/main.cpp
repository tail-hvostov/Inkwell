#include "Application.h"
#include "MainWindow.h"

//hInstance - ���������� ������.
//hPrevInstance - �����.
//pCmdLine - ��������� ��������� ������ � ���� ������ �������.
//nCmdShow - ����, �����������, �������� �� �������� ���� ����������
//��������, ���������� ��� ������������ � �������  ������.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PWSTR pCmdLine, int nCmdShow) {
	Application::init_app(hInstance);
	MainWindow* main_window = MainWindow::create();
	if (!main_window->is_sufficient()) {
		delete main_window;
		return 0;
	}
	main_window->show(nCmdShow);
	Application::mainloop();
	return 0;
}