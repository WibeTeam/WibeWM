#pragma once

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
}
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Types.hpp"

struct Fnt {
	Display*   Display;
	uint       Height;
	XftFont*   Xfont;
	FcPattern* Pattern;
};

struct Drw {
	uint      Width;
	uint      Height;
	Display*  Display;
	int       Screen;
	Window    Root;
	Drawable  drawable;
	GC        Gc;
	XftColor* Scheme;
	Fnt*      Fonts;

	Drw(Display* display, int screen, Window window, uint width, uint height);
	~Drw();
	void Resize(uint width, uint height);

};