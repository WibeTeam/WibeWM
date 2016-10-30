#pragma once

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
}
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bits/forward_list.h>

#include "Types.hpp"

struct Drw;

struct Fnt {
	Display*   display;
	unsigned  height;
	XftFont*   xfont;
	FcPattern* pattern;

	Fnt(Display* disp, unsigned h, XftFont* font, FcPattern* pattern);
	~Fnt();
};

struct Drw {
	unsigned      width;
	unsigned      height;
	Display*       display;
	int            screen;
	Window         root;
	Drawable       drawable;
	GC             gc;
	XftColor*      scheme;
	Fnt*           fonts;
	std::forward_list<Fnt*> fonts;

	Fnt* Create(const char* fontname, FcPattern* fontpattern);

	Drw(Display* disp, int scr, Window win, unsigned w, unsigned h);
	~Drw();

	void Resize(unsigned width, unsigned height);
	unsigned GetWidth(const char* text);
	int Text(int x, int y, unsigned w, unsigned h, unsigned lpad, const char* text, bool invert)
};