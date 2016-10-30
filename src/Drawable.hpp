#pragma once

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
}
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <forward_list>

#include "Types.hpp"

struct Drw;

struct Cur {
	Cursor cursor;
};

struct Fnt {
	Display*   display;
	unsigned   height;
	XftFont*   xfont;
	FcPattern* pattern;

	Fnt(Display* disp, unsigned h, XftFont* font, FcPattern* pattern);
	~Fnt();

	void GetExts(const char* text, unsigned len, unsigned* w, unsigned* h);
};

struct Drw {
	unsigned                width;
	unsigned                height;
	Display*                display;
	int                     screen;
	Window                  root;
	Drawable                drawable;
	GC                      gc;
	XftColor*               scheme;
	std::forward_list<Fnt*> fonts;

	Fnt* Create(const char* fontname, FcPattern* fontpattern);
	void Create(XftColor* dest, const char* color);
	XftColor* Create(const char* clrnames[], size_t clrcount);
	Cur* CreateCursor(int shape);
	void DestroyCursor(Cur* cursor);

	Drw(Display* disp, int scr, Window win, unsigned w, unsigned h);
	~Drw();

	void Resize(unsigned width, unsigned height);
	unsigned GetWidth(const char* text);
	void Rect(int x, int y, unsigned w, unsigned h, int filled, bool invert);
	int Text(int x, int y, unsigned w, unsigned h, unsigned lpad, const char* text, bool invert);

	void Map(Window win, int x, int y, unsigned w, unsigned h);
};