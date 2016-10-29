#include <iostream>
#include <unistd.h>

#include "WindowManager.hpp"

int main(int argc, char *argv[])
{
	std::shared_ptr<WindowManager> wm = WindowManager::Instance();
	if (!wm) {
		std::cerr << "Couldn't open display\n";
		return -1;
	}
	wm->Run();
	if (wm->NeedRestart()) {
		execvp(argv[0], argv);
	}
	return 0;
}
