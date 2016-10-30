#include "Drawable.hpp"

Drw::Drw(Display* display, int screen, Window window, uint width, uint height) :
		Width(w),
		Height(h),
		Display(display),
		Screen(screen),
		Root(root),
		drawable(XCreatePixmap(Display, Root, Width, Height, DefaultDepth(Display, Screen))),
		Gc(XCreateGC(Display, Root, 0, nullptr)) {
	XSetLineAttributes(Display, GC, 1, LineSolid, CapButt, JoinMiter);
}

Drw::~Drw() {
	XFreePixmap(Display, drawable);
	XFreeGC(Display, GC);
}

void Drw::Resize(uint width, uint height) {
	if (drawable)
		XFreePixmap(Display, drawable);
	drawable = XCreatePixmap(Display, Root, Width, Height, DefaultDepth(Display, Screen));
}
