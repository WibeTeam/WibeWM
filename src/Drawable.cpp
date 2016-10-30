#include "Drawable.hpp"
#include <algorithm>

#define UTF_SIZ     4
static const long InvalidUTF = 0xFFFD;
static const unsigned char utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
static const unsigned char utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static const long utfmin[UTF_SIZ + 1] = {       0,    0,  0x80,  0x800,  0x10000};
static const long utfmax[UTF_SIZ + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

static long
utf8decodebyte(const char c, size_t *i)
{
	for (*i = 0; *i < (UTF_SIZ + 1); ++(*i))
		if (((unsigned char)c & utfmask[*i]) == utfbyte[*i])
			return (unsigned char)c & ~utfmask[*i];
	return 0;
}

static size_t
utf8validate(long *u, size_t i)
{
	if (!(utfmin[i] <= *u && *u <= utfmax[i]) || (0xD800 <= *u && *u <= 0xDFFF))
		*u = InvalidUTF;
	for (i = 1; *u > utfmax[i]; ++i)
		;
	return i;
}

static size_t
utf8decode(const char *c, long *u, size_t clen)
{
	size_t i, j, len, type;
	long udecoded;

	*u = InvalidUTF;
	if (!clen)
		return 0;
	udecoded = utf8decodebyte(c[0], &len);
	if (!(1 <= len && len <= UTF_SIZ))
		return 1;
	for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
		udecoded = (udecoded << 6) | utf8decodebyte(c[i], &type);
		if (type)
			return j;
	}
	if (j < len)
		return 0;
	*u = udecoded;
	utf8validate(u, len);

	return len;
}

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

void Drw::Create(XftColor* dest, const char* color) {
 	if (!dest || !color)
		return;

	if (!XftColorAllocName(display, DefaultVisual(display, screen),
	                       DefaultColormap(display, screen),
	                       color, dest))
		exit(-2);
}

XftColor* Drw::Create(const char* clrnames[], size_t clrcount) {
	size_t i;
	XftColor* ret = (XftColor*)calloc(clrcount, sizeof(XftColor));

	if (!clrnames || clrcount < 2 || !ret)
		return nullptr;

	for (i = 0; i < clrcount; i++)
		Create(&ret[i], clrnames[i]);
	return ret;
}

Cur* Drw::CreateCursor(int shape) {
	Cur* cur = (Cur*)calloc(1, sizeof(Cur));
	if (!cur)
		return nullptr;

	cur->cursor = XCreateFontCursor(display, shape);

	return cur;
}

void Drw::DestroyCursor(Cur* cursor) {
	if (!cursor)
		return;

	XFreeCursor(display, cursor->cursor);
	free(cursor);

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
	XftDraw *d = nullptr;
	size_t i, len;
	int utf8strlen, utf8charlen, render = x || y || w || h;
	long utf8codepoint = 0;
	const char *utf8str;
	FcCharSet *fccharset;
	FcPattern *fcpattern;
	FcPattern *match;
	XftResult result;

	if ((render && !scheme) || !text || fonts.empty())
		return 0;

	if (!render) {
		w = ~w;
	} else {
		XSetForeground(display, gc, scheme[invert ? (int)SchemeIndex::Foreground : (int)SchemeIndex::Background].pixel);
		XFillRectangle(display, drawable, gc, x, y, w, h);
		d = XftDrawCreate(display, drawable, DefaultVisual(display, screen), DefaultColormap(display, screen));
		x += lpad;
		w -= lpad;
	}

	Fnt* usedFont = fonts.front();
	Fnt *nextfont;
	bool charexists = 0;
	while (1) {
		utf8strlen = 0;
		utf8str = text;
		nextfont = nullptr;
		while (*text) {
			utf8charlen = utf8decode(text, &utf8codepoint, 4);
			for (auto currentFont = fonts.begin(); currentFont != fonts.end(); ++currentFont) {
				charexists = charexists || XftCharExists(display, (*currentFont)->xfont, utf8codepoint);
				if (charexists) {
					if (*currentFont == usedFont) {
						utf8strlen += utf8charlen;
						text += utf8charlen;
					} else {
						nextfont = *currentFont;
					}
					break;
				}
			}

			if (!charexists || nextfont)
				break;
			else
				charexists = false;
		}

		if (utf8strlen) {
			usedFont->GetExts(utf8str, utf8strlen, &ew, nullptr);
			for (len = std::min((s64)utf8strlen, (s64)(sizeof(buf) - 1)); len && ew > w; len--)
				usedFont->GetExts(utf8str, len, &ew, nullptr);

			if (len) {
				memcpy(buf, utf8str, len);
				buf[len] = '\0';
				if (len < utf8strlen) {
					for (i = len; i && i > len - 3; buf[--i] = '.');
				}

				if (render) {
					ty = y + (h - usedFont->height) / 2 + usedFont->xfont->ascent;
					XftDrawStringUtf8(d, &scheme[invert ? (int)SchemeIndex::Foreground : (int)SchemeIndex::Background],
					                  usedFont->xfont, x, ty, (XftChar8 *)buf, len);
				}
				x += ew;
				w -= ew;
			}
		}

		if (!*text) {
			break;
		} else if (nextfont) {
			charexists = false;
			usedFont = nextfont;
		} else {
			charexists = true;

			fccharset = FcCharSetCreate();
			FcCharSetAddChar(fccharset, utf8codepoint);

			if (fonts.front()->pattern) {
				exit(-2);
			}

			fcpattern = FcPatternDuplicate(fonts.front()->pattern);
			FcPatternAddCharSet(fcpattern, FC_CHARSET, fccharset);
			FcPatternAddBool(fcpattern, FC_SCALABLE, FcTrue);

			FcConfigSubstitute(nullptr, fcpattern, FcMatchPattern);
			FcDefaultSubstitute(fcpattern);
			match = XftFontMatch(display, screen, fcpattern, &result);

			FcCharSetDestroy(fccharset);
			FcPatternDestroy(fcpattern);

			if (match) {
				usedFont = Create(nullptr, match);
				if (usedFont && XftCharExists(display, usedFont->xfont, utf8codepoint)) {
					fonts.push_back(usedFont);
				} else {
					usedFont = fonts.front();
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

