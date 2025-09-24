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

	void set_text_clipboard_data(const char* text) {
		if (!OpenClipboard(NULL)) {
			return;
		}
		EmptyClipboard();
		int text_length = strlen(text);
		if (text_length) {
			HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, text_length + 1); 
			if (!hglbCopy) {
				goto Ending;
			}
			LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
			memcpy(lptstrCopy, text, text_length); 
			lptstrCopy[text_length] = 0; 
			GlobalUnlock(hglbCopy);
			SetClipboardData(CF_TEXT, hglbCopy);
		}
	Ending:
		CloseClipboard();
	}

	char* get_text_clipboard_data() {
		char* result;
		if (!IsClipboardFormatAvailable(CF_TEXT)) {
            goto Failure; 
		}
        if (!OpenClipboard(NULL))  {
            goto Failure;
		}
		HGLOBAL hglb = GetClipboardData(CF_TEXT);
		if (!hglb) {
			CloseClipboard();
			goto Failure;
		}
		LPTSTR lptstr = (LPSTR)GlobalLock(hglb);
		if (!lptstr) {
			CloseClipboard();
			goto Failure;
		}
		int text_length = strlen(lptstr);
		result = new char[text_length + 1];
		memcpy(result, lptstr, text_length);
		result[text_length] = 0;
		GlobalUnlock(hglb);
		CloseClipboard();
		return result;
	Failure:
		result = new char[1];
		result[0] = 0;
		return result;
	}

	namespace Win32 {
		HINSTANCE get_hinstance() {
			return hInstance;
		}
	};
};