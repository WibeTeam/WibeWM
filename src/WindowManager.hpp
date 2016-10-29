#pragma once

#include <memory>
#include "Connection.hpp"
#include "Types.hpp"
#include "Window.hpp"
#include "Workspace.hpp"

class ILayout;

class WindowManager {
public:
	static WindowManager& Instance();

	static void quit(const Arg* args);
	static void restart(const Arg* args);
	static void spawn(const Arg* args);

	void          Run();
	bool          NeedRestart()  const;
	Window        GetRootWindow() const;

	~WindowManager();

private:
	WindowManager();
	WindowManager(const WindowManager& other)       = delete;
	WindowManager(const WindowManager&& other)      = delete;
	WindowManager& operator=(const WindowManager&) = delete;


	bool             _needRestart;
	bool             _exit;

	ILayout* _layout;

	int               _screenNumber;
	ScreenPtr         _rootScreen;
	Workspace*        _currentWorkspace;
};
