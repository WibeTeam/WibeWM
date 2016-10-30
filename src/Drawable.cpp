#include "Drawable.hpp"
#include <algorithm>

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

void Fnt::GetExts(const char* text, unsigned len, unsigned* w, unsigned* h) {
	if (!text)
		return;

	XGlyphInfo ext;

	XftTextExtentsUtf8(display, xfont, (XftChar8 *)text, len, &ext);
	if (w)
		*w = ext.xOff;
	if (h)
		*h = height;
}

Fnt* Drw::Create(const char* fontname, FcPattern* fontpattern) {
	XftFont* xftFont = nullptr;
	FcPattern* pattern = nullptr;
	if (fontname) {
		if (!(xftFont = XftFontOpenName(display, screen, fontname))) {
			return nullptr;
		}
		if (!(pattern = FcNameParse((FcChar8 *) fontname))) {
			XftFontClose(display, xftFont);
			return nullptr;
		}
	} else if (fontpattern) {
		if (!(xftFont = XftFontOpenPattern(display, fontpattern))) {
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

unsigned Drw::GetWidth(const char* text) {
	if (fonts.empty() || !text)
		return 0;
	return Text(0, 0, 0, 0, 0, text, false);
}

void Drw::Rect(int x, int y, unsigned w, unsigned h, int filled, bool invert) {
	if (!scheme)
		return;
	XSetForeground(display, gc, scheme[invert ? (int)SchemeIndex::Background
	                                          : (int)SchemeIndex::Foreground].pixel);
	if (!filled) {
		--w;
		--h;
	}
	XDrawRectangle(display, drawable, gc, x, y, w - 1, h - 1);
}

int Drw::Text(int x, int y, unsigned w, unsigned h, unsigned lpad, const char *text, bool invert) {
	char buf[1024];
	int ty;
	unsigned int ew;
	XftDraw *d = NULL;
	Fnt *usedfont, *curfont, *nextfont;
	size_t i, len;
	int utf8strlen, utf8charlen, render = x || y || w || h;
	long utf8codepoint = 0;
	const char *utf8str;
	FcCharSet *fccharset;
	FcPattern *fcpattern;
	FcPattern *match;
	XftResult result;
	int charexists = 0;

	if ((render && !scheme) || !text || !fonts)
		return 0;

	if (!render) {
		w = ~w;
	} else {
		XSetForeground(display, gc, scheme[invert ?
		                                   (int)SchemeIndex::Foreground :
		                                   (int)SchemeIndex::Background].pixel);
		XFillRectangle(display, drawable, gc, x, y, w, h);
		d = XftDrawCreate(display, drawable, DefaultVisual(display, screen), DefaultColormap(display, screen));
		x += lpad;
		w -= lpad;
	}

	usedfont = fonts;
	while (1) {
		utf8strlen = 0;
		utf8str = text;
		nextfont = nullptr;
		while (*text) {
			utf8charlen = utf8decode(text, &utf8codepoint, 4);
			for (curfont = fonts; curfont; curfont = curfont->next) {
				charexists = charexists || XftCharExists(display, curfont->xfont, utf8codepoint);
				if (charexists) {
					if (curfont == usedfont) {
						utf8strlen += utf8charlen;
						text += utf8charlen;
					} else {
						nextfont = curfont;
					}
					break;
				}
			}

			if (!charexists || nextfont)
				break;
			else
				charexists = 0;
		}

		if (utf8strlen) {
			drw_font_getexts(usedfont, utf8str, utf8strlen, &ew, NULL);
			/* shorten text if necessary */
			for (len = std::min(utf8strlen, sizeof(buf) - 1); len && ew > w; len--)
				drw_font_getexts(usedfont, utf8str, len, &ew, NULL);

			if (len) {
				memcpy(buf, utf8str, len);
				buf[len] = '\0';
				if (len < utf8strlen)
					for (i = len; i && i > len - 3; buf[--i] = '.')
						; /* NOP */

				if (render) {
					ty = y + (h - usedfont->height) / 2 + usedfont->xfont->ascent;
					XftDrawStringUtf8(d, &scheme[invert ?
		                                  (int)SchemeIndex::Foreground :
		                                  (int)SchemeIndex::Background],
					                  usedfont->xfont, x, ty, (XftChar8 *)buf, len);
				}
				x += ew;
				w -= ew;
			}
		}

		if (!*text) {
			break;
		} else if (nextfont) {
			charexists = 0;
			usedfont = nextfont;
		} else {
			/* Regardless of whether or not a fallback font is found, the
			 * character must be drawn. */
			charexists = 1;

			fccharset = FcCharSetCreate();
			FcCharSetAddChar(fccharset, utf8codepoint);

			if (!fonts->pattern) {
				exit(-2);
			}

			fcpattern = FcPatternDuplicate(drw->fonts->pattern);
			FcPatternAddCharSet(fcpattern, FC_CHARSET, fccharset);
			FcPatternAddBool(fcpattern, FC_SCALABLE, FcTrue);

			FcConfigSubstitute(NULL, fcpattern, FcMatchPattern);
			FcDefaultSubstitute(fcpattern);
			match = XftFontMatch(drw->dpy, drw->screen, fcpattern, &result);

			FcCharSetDestroy(fccharset);
			FcPatternDestroy(fcpattern);

			if (match) {
				usedfont = xfont_create(drw, NULL, match);
				if (usedfont && XftCharExists(drw->dpy, usedfont->xfont, utf8codepoint)) {
					for (curfont = drw->fonts; curfont->next; curfont = curfont->next)
						; /* NOP */
					curfont->next = usedfont;
				} else {
					xfont_free(usedfont);
					usedfont = drw->fonts;
				}
			}
		}
	}
	if (d)
		XftDrawDestroy(d);

	return x + (render ? w : 0);
}

void Drw::Map(Window win, int x, int y, unsigned w, unsigned h) {
	XCopyArea(display, drawable, win, gc, x, y, w, h, x, y);
	XSync(display, False);
}

