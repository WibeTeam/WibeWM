#pragma once

extern "C" {
#include <xcb/xcb.h>
}

#include "types.hpp"

class WindowManager {
public:
	static WindowManager& Instance();

	void Run();
	bool NeedRestart() const;

	~WindowManager();

private:
	WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator=(const WindowManager&) = delete;


	bool             _needRestart;

	ConnectionPtr     _connection;
	int               _screenNumber;
	ScreenPtr         _screen;
};
