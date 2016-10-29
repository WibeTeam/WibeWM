#pragma once

extern "C" {
#include <xcb/xcb.h>
}

#include <memory>
#include "Layouts.hpp"
#include "types.hpp"

class WindowManager {
public:
	static WindowManager& Instance();

	static void exit();
	static void restart();

	void Run();
	bool NeedRestart() const;

	~WindowManager();

private:
	WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator=(const WindowManager&) = delete;


	bool             _needRestart;
	bool             _exit;

	std::unique_ptr<ILayout> _layout;

	ConnectionPtr     _connection;
	int               _screenNumber;
	ScreenPtr         _rootScreen;

};
