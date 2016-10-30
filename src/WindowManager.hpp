#pragma once

extern "C" {
#include <X11/Xlib.h>
}

#include <memory>
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

	static std::shared_ptr<WindowManager> _self;

	Display*                               _display;
	int                                    _screen;
	const Window                          _root;
	XRectangle                             _rootRect;
	bool                                  _needRestart;
	bool                                  _exit;
};
