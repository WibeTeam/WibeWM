#include "WindowManager.hpp"
#include "config.hpp"
#include "Layouts.hpp"

#include <stdexcept>
#include <unistd.h>

std::shared_ptr<WindowManager> WindowManager::_self = nullptr;

std::shared_ptr<WindowManager> WindowManager::Instance() {
	if (!_self) {
		Display* display = XOpenDisplay(nullptr);
		if (!display)
			throw
		_self = std::make_shared<WindowManager>(display);
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
}

WindowManager::WindowManager(Display* display)
		:_needRestart(false)
		,_exit(false) {

}
