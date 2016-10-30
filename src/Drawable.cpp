#include "Drawable.hpp"

Fnt::Fnt(Display* disp, unsigned h, XftFont* font, FcPattern* pattern) :
		display(disp),
		height(h),
		xfont(font),
		pattern(pattern) {
}

Fnt::~Fnt() {
	if (pattern)
		FcPatternDestroy(pattern);
	XftFontClose(display, xfont);
}

unsigned Fnt::GetWidth(const char* text) {
	return 0;
}

Fnt* Drw::Create(const char* fontname, FcPattern* fontpattern) {
	XftFont* xftFont = nullptr;
	FcPattern* pattern = nullptr;
	if (fontname) {
		if (!(xftFont = XftFontOpenName(display, screen, fontname))) {
			fprintf(stderr, "error, cannot load font from name: '%s'\n", fontname);
			return nullptr;
		}
		if (!(pattern = FcNameParse((FcChar8 *) fontname))) {
			fprintf(stderr, "error, cannot parse font name to pattern: '%s'\n", fontname);
			XftFontClose(display, xftFont);
			return nullptr;
		}
	} else if (fontpattern) {
		if (!(xftFont = XftFontOpenPattern(display, fontpattern))) {
			fprintf(stderr, "error, cannot load font from pattern.\n");
			return nullptr;
		}
	} else {
		exit(-2);
	}
	return new Fnt(display, (int)(xftFont->ascent + xftFont->descent), xftFont, pattern);
}


Drw::Drw(Display* disp, int scr, Window win, unsigned w, unsigned h) :
		width(w),
		height(h),
		display(display),
		screen(screen),
		root(root),
		drawable(XCreatePixmap(display, root, width, height, DefaultDepth(display, screen))),
		gc(XCreateGC(display, root, 0, nullptr)) {
	XSetLineAttributes(display, gc, 1, LineSolid, CapButt, JoinMiter);
}

Drw::~Drw() {
	XFreePixmap(display, drawable);
	XFreeGC(display, gc);
}

void Drw::Resize(uint width, uint height) {
	if (drawable)
		XFreePixmap(display, drawable);
	drawable = XCreatePixmap(display, root, width, height, DefaultDepth(display, screen));
}
