#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <windows.h>

namespace Application {
	void init_app(HINSTANCE hInstance);
	//Если menu_id < 0, то меню не будет.
	void register_window_class(const char* name, int menu_id);
	void mainloop();

	namespace Win32 {
		HINSTANCE get_hinstance();
	};
};

#endif