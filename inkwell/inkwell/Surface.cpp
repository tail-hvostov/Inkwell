#include "Surface.h"
#include "Application.h"

void query_bitmap_dimensions(HBITMAP hbitmap, LONG* w, LONG* h) {
	if (hbitmap) {
		BITMAP bm;
		GetObject(hbitmap, (int) sizeof bm, &bm);
		*w = bm.bmWidth;
		*h = bm.bmHeight;
	}
}

Surface::Surface(int resource) {
	bitmap = LoadBitmap(Application::Win32::get_hinstance(), MAKEINTRESOURCE(resource));
	query_bitmap_dimensions(bitmap, &w, &h);
}

void Surface::blit(HDC hdc, int x, int y) {
	HDC hMemDC = CreateCompatibleDC(NULL);
	HBITMAP old_bitmap = (HBITMAP)SelectObject(hMemDC, bitmap);
	BitBlt(hdc, x, y, w, h, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, old_bitmap);
	DeleteDC(hMemDC);
}

Surface::~Surface() {
	DeleteObject(bitmap);
}

void Surface::query_dimensions(LONG* w, LONG* h) {
	*w = this->w;
	*h = this->h;
}

LONG Surface::get_h() {
	return h;
}

LONG Surface::get_w() {
	return w;
}