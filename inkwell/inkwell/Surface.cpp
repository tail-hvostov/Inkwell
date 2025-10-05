#include "Surface.h"
#include "Application.h"

Surface::Surface(int resource) {
	bitmap = LoadBitmap(Application::Win32::get_hinstance(), MAKEINTRESOURCE(resource));
}

Surface::~Surface() {
	DeleteObject(bitmap);
}