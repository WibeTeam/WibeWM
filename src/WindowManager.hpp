#pragma once

extern "C" {
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
}

#include <forward_list>
#include <memory>

#include "Drawable.hpp"
#include "Monitor.hpp"
#include "Types.hpp"

class WindowManager {
public:
	static std::shared_ptr<WindowManager> Instance();

	static void quit(const Arg* args);
	static void restart(const Arg* args);
	static void spawn(const Arg* args);

	void          Run();
	bool          NeedRestart()  const;

	~WindowManager();

private:
	WindowManager(Display* display);
	WindowManager(const WindowManager& other)       = delete;
	WindowManager(const WindowManager&& other)      = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	void UpdateBars();
    void UpdateStatus();
    void DrawBar();

	static std::shared_ptr<WindowManager>  _self;

	Display*                               _display;
	int                                    _screen;
	const Window                           _root;
    Rectangle                              _rootRect;
	std::forward_list<Monitor*>            _monitors;
	Atom                                   _wmatom[(int)WM::Size];
	Atom                                   _netatom[(int)Net::Size];
	Cur*                                   _cursor[(int)CursorState::Size];
	XftColor*                              _scheme[(int)ColorScheme::Size];
	bool                                   _needRestart;
	bool                                   _exit;
};
