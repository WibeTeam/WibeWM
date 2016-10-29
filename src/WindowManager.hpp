#pragma once

extern "C" {
#include <xcb/xcb.h>
}

#include <memory>
#include "Types.hpp"

class ILayout;

class WindowManager {
public:
	static WindowManager& Instance();
	
	static void quit(const Arg* args);
	static void restart(const Arg* args);
	static void spawn(const Arg* args);

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
