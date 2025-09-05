#include "Application.h"
#include "ProtoWindow.h"

namespace {
	HINSTANCE hInstance;
};

namespace Application {
	void init_app(HINSTANCE hInstanceOuter) {
		hInstance = hInstanceOuter;
	}

	void register_window_class(const char* name, int menu_id) {
		WNDCLASS wc = {};
		wc.lpfnWndProc = ProtoWindow::foundation_window_proc;
		wc.hInstance = hInstance;
		wc.lpszClassName = name;
		if (menu_id >= 0) {
			wc.lpszMenuName = MAKEINTRESOURCE(menu_id);
		}
		RegisterClass(&wc);
	}

	void mainloop() {
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
	}

	namespace Win32 {
		HINSTANCE get_hinstance() {
			return hInstance;
		}
	};
};