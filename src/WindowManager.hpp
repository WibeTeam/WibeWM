#pragma once

extern "C" {
#include <xcb/xcb.h>
}

#include "types.hpp"

class WindowManager {
public:
	static WindowManager& Instance();
	
	static void exit() { Instance()._exit = true; }

	void Run();
	bool NeedRestart() const;

	~WindowManager();

private:
	WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator=(const WindowManager&) = delete;


	bool             _needRestart;
	bool             _exit = false;

	ConnectionPtr     _connection;
	int               _screenNumber;
	ScreenPtr         _rootScreen;
};
