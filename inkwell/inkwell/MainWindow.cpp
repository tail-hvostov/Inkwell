#include "MainWindow.h"
#include "Application.h"
#include "resource.h"

#define TEXTBOX_MARGIN 10
#define BG_COLOR RGB(0, 200, 60)
#define FREEZE_DELAY 5000
#define SPRITE_VELOCITY 80

namespace {
	const char CLASS_NAME[] = "Inkwell Main Class";
	const char MAIN_WINDOW_NAME[] = "Inkwell";
	bool is_class_registered = false;
};

MainWindow* MainWindow::create() {
	if (!is_class_registered) {
		Application::register_window_class(CLASS_NAME, IDR_MENU1);
	}
	return new MainWindow();
}

MainWindow::MainWindow() : ProtoWindow(CLASS_NAME, MAIN_WINDOW_NAME,
										CW_USEDEFAULT, CW_USEDEFAULT)
{}

void MainWindow::on_paint(PAINTSTRUCT* ps) {
	//Структура для рисования клиентской части окна.
	HDC hdc = ps->hdc;
	HBRUSH brush = CreateSolidBrush(BG_COLOR);
	FillRect(hdc, &ps->rcPaint, brush);
	DeleteObject(brush);
	if (sprite_mode) {
		sprite->blit(hdc, sprite_x, sprite_y);
	}
}

LRESULT MainWindow::on_close(WPARAM wParam, LPARAM lParam) {
	if (unsaved_changes_prompt()) {
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

void MainWindow::on_copy() {
	char* text = text_box->get_selected_text();
	Application::set_text_clipboard_data(text);
	delete[] text;
}

void MainWindow::on_cut() {
	char* text = text_box->cut_selected_text();
	Application::set_text_clipboard_data(text);
	delete[] text;
}

void MainWindow::on_place() {
	char* text = Application::get_text_clipboard_data();
	text_box->paste_text(text);
	delete[] text;
}

bool MainWindow::save_text(const char* file_name) {
	bool result = false;
	HANDLE file = CreateFile(file_name, GENERIC_WRITE, 0, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file != INVALID_HANDLE_VALUE) {
		char* text = text_box->get_text();
		DWORD written_bytes;
		if (WriteFile(file, text, strlen(text), &written_bytes, NULL)) {
			result = true;
		}
		else {
			show_message("Не удалось записать файл.");
		}
		delete[] text;
		CloseHandle(file);
	}
	else {
		show_message("Не удалось открыть файл.");
	}
	return result;
}

bool MainWindow::on_save() {
	bool result = false;
	OPENFILENAME ofn;
	char* name_buf = new char[256];

	memset(&ofn, 0, sizeof(OPENFILENAME));
	memset(name_buf, 0, 256);
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = Application::Win32::get_hinstance();
	ofn.lpstrFile = name_buf;
	ofn.nMaxFile = 256;
	if (GetSaveFileName(&ofn)) {
		result = save_text(name_buf);
	}
	delete[] name_buf;
	if (unsaved_changes) {
		unsaved_changes = !result;
	}
	return result;
}

void MainWindow::on_menu_press(WORD item) {
	delay_animation();
	switch (item) {
	case ID_MCOPY:
		on_copy();
		break;
	case ID_MCUT:
		on_cut();
		break;
	case ID_MPLACE:
		on_place();
		break;
	case ID_MFSAVE:
		on_save();
		break;
	case ID_MFNEW:
		on_new_document();
		break;
	case ID_MFOPEN:
		on_open();
		break;
	case ID_MQUIT:
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case ID_MABOUT:
		DialogBox(Application::Win32::get_hinstance(), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutProc);
		break;
	default:
		show_message("This operation has not been implemented.");
		break;
	}
}

void MainWindow::calc_textbox_rect(RECT* rect) {
	GetClientRect(hwnd, rect);
	rect->left = TEXTBOX_MARGIN;
	rect->right -= TEXTBOX_MARGIN;
	if (rect->right < rect->left) {
		rect->right = rect->left;
	}
	rect->top = TEXTBOX_MARGIN;
	rect->bottom -= TEXTBOX_MARGIN;
	if (rect->bottom < rect->top) {
		rect->bottom = rect->top;
	}
}

void MainWindow::init_sprite() {
	sprite.reset(new Surface(IDB_DEMSPRITE));
	sprite_x = 40;
	sprite_y = 60;
	sprite_vx = SPRITE_VELOCITY;
	sprite_vy = SPRITE_VELOCITY;
	sprite_mode = false;
	freeze_timer.reset(new SuperTimer([this]() {shift_mode();}, FREEZE_DELAY));
	cooldown_timer.reset(new SuperTimer([this]() {
		sprite_stamp = GetTickCount();
		anim_timer->start();
		cooldown_timer->stop();
	}, 80, false));
	anim_timer.reset(new SuperTimer([this]() {auto_sprite_move();}, 40, false));
}

void MainWindow::on_create() {
	RECT rect;
	calc_textbox_rect(&rect);
	text_box.reset(new TextBox(rect.left, rect.top,
								rect.right - rect.left,
								rect.bottom - rect.top,
								this));
	unsaved_changes = false;
	init_sprite();
}

void MainWindow::on_resize(UINT width, UINT height) {
	text_box->set_size(width - TEXTBOX_MARGIN * 2, height - TEXTBOX_MARGIN * 2);
}

void MainWindow::on_control_notification(ProtoControl* control, WORD notification) {
	if (control == text_box.get()) {
		if (notification == EN_CHANGE) {
			unsaved_changes = true;
		}
	}
}

bool MainWindow::unsaved_changes_prompt() {
	bool result = true;
	if (unsaved_changes) {
		if (MessageBox(hwnd, "Вы не сохранили последние изменения. Сохранить их?", MAIN_WINDOW_NAME, MB_OKCANCEL) == IDOK) {
			result = on_save();
		}
	}
	return result;
}

void MainWindow::on_new_document() {
	if (unsaved_changes_prompt()) {
		text_box->clear();
		unsaved_changes = false;
	}
}

void MainWindow::load_text(const char* file_name) {
	HANDLE file = CreateFile(file_name, GENERIC_READ, 0, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file != INVALID_HANDLE_VALUE) {
		DWORD read_bytes;
		DWORD file_length;
		file_length = GetFileSize(file, NULL);
		int f = GetLastError();
		char* text = new char[file_length + 1];
		if (ReadFile(file, text, file_length, &read_bytes, NULL)) {
			text[file_length] = 0;
			text_box->set_text(text);
			unsaved_changes = false;
		}
		else {
			show_message("Не удалось прочитать файл.");
		}
		delete[] text;
		CloseHandle(file);
	}
	else {
		show_message("Не удалось открыть файл.");
	}
}

void MainWindow::on_open() {
	if (unsaved_changes_prompt()) {
		OPENFILENAME ofn;
		char* name_buf = new char[256];

		memset(&ofn, 0, sizeof(OPENFILENAME));
		memset(name_buf, 0, 256);
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hwnd;
		ofn.hInstance = Application::Win32::get_hinstance();
		ofn.lpstrFile = name_buf;
		ofn.nMaxFile = 256;
		if (GetOpenFileName(&ofn)) {
			load_text(name_buf);
		}
		delete[] name_buf;
	}
}

LRESULT MainWindow::on_raw_msg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CHAR:
		switch (wParam) {
		case L'w':
			move_sprite_up();
			break;
		case L'a':
			move_sprite_left();
			break;
		case L's':
			move_sprite_down();
			break;
		case L'd':
			move_sprite_right();
			break;
		}
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void MainWindow::on_keydown(WPARAM key, LPARAM params) {
	if (sprite_mode) {
		switch (key) {
		case VK_UP:
			move_sprite_up();
			break;
		case VK_LEFT:
			move_sprite_left();
			break;
		case VK_RIGHT:
			move_sprite_right();
			break;
		case VK_DOWN:
			move_sprite_down();
			break;
		case 'W':
		case 'A':
		case 'S':
		case 'D':
			break;
		default:
			shift_mode();
			break;
		}
	}
}

void MainWindow::shift_mode() {
	sprite_mode = !sprite_mode;
	if (sprite_mode) {
		sprite_stamp = GetTickCount();
		freeze_timer->stop();
		text_box->set_visible(false);
		anim_timer->start();
	}
	else {
		text_box->set_visible(true);
		freeze_timer->start();
		cooldown_timer->stop();
		anim_timer->stop();
	}
}

void MainWindow::shift_sprite(int vx, int vy) {
	DWORD old_stamp = sprite_stamp;
	sprite_stamp = GetTickCount();
	long delta = sprite_stamp - old_stamp;
	long shift = ((delta / 1000.0) * vx);
	sprite_x += shift;
	shift = ((delta / 1000.0) * vy);
	sprite_y += shift;
}

void MainWindow::auto_sprite_move() {
	LONG client_w, client_h, sprite_w, sprite_h;
	sprite->query_dimensions(&sprite_w, &sprite_h);
	query_client_dims(&client_w, &client_h);
	shift_sprite(sprite_vx, sprite_vy);
	long overrun;
	overrun = sprite_x + sprite_w - client_w;
	if (overrun > 0) {
		sprite_x -= overrun * 2;
		sprite_vx = -SPRITE_VELOCITY;
	}
	else if (sprite_x < 0) {
		sprite_x *= -1;
		sprite_vx = SPRITE_VELOCITY;
	}

	overrun = sprite_y + sprite_h - client_h;
	if (overrun > 0) {
		sprite_y -= overrun * 2;
		sprite_vy = -SPRITE_VELOCITY;
	}
	else if (sprite_y < 0) {
		sprite_y *= -1;
		sprite_vy = SPRITE_VELOCITY;
	}
	this->invalidate_client();
}

void MainWindow::move_sprite_down() {
	anim_timer->stop();
	cooldown_timer->start();
	shift_sprite(0, SPRITE_VELOCITY);
	invalidate_client();
}

void MainWindow::move_sprite_left() {
	anim_timer->stop();
	cooldown_timer->start();
	shift_sprite(-SPRITE_VELOCITY, 0);
	if (sprite_x < 0) {
		sprite_x = 0;
	}
	invalidate_client();
}

void MainWindow::move_sprite_up() {
	anim_timer->stop();
	cooldown_timer->start();
	shift_sprite(0, -SPRITE_VELOCITY);
	if (sprite_y < 0) {
		sprite_y = 0;
	}
	invalidate_client();
}

void MainWindow::move_sprite_right() {
	LONG cw, cy;
	query_client_dims(&cw, &cy);
	anim_timer->stop();
	cooldown_timer->start();
	shift_sprite(SPRITE_VELOCITY, 0);
	int overrun = sprite_x + sprite->get_w() - cw;
	if (overrun > 0) {
		sprite_x -= overrun;
	}
	invalidate_client();
}

void MainWindow::delay_animation() {
	if (sprite_mode) {
		shift_mode();
	}
	else {
		freeze_timer->start();
	}
}