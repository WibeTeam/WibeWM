#include "WindowManager.hpp"

#include <unistd.h>

std::shared_ptr<WindowManager> WindowManager::_self = nullptr;

std::shared_ptr<WindowManager> WindowManager::Instance() {
	if (!_self) {
		Display* display = XOpenDisplay(nullptr);
		if (!display)
			return nullptr;
		_self = std::shared_ptr<WindowManager>(new WindowManager(display));
	}
	return _self;
}

void WindowManager::quit(const Arg* args) {
	Instance()->_exit = true;
}

void WindowManager::restart(const Arg* args) {
	Instance()->_needRestart = true;
	Instance()->_exit = true;
}

void WindowManager::spawn(const Arg* args) {
	if (fork() == 0) {
		setsid();
		execvp(((char**)args->v)[0], (char**)args->v);
		printf("WindowManager: execvp %s", ((char**)args->v)[0]);
		perror("failed");
		exit(EXIT_SUCCESS);
	}
}

void WindowManager::Run() {

}

bool WindowManager::NeedRestart() const {
	return _needRestart;
}

WindowManager::~WindowManager() {
	XCloseDisplay(_display);
}

WindowManager::WindowManager(Display* display) :
		_display(display),
		_screen(DefaultScreen(_display)),
		_root(DefaultRootWindow(_display)),
		_rootRect({0, 0, DisplayWidth(_display, _screen), DisplayWidth(_display, _screen)}),
		_needRestart(false),
		_exit(false) {
}
