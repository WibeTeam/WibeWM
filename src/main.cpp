#include <iostream>
#include <unistd.h>

#include "WindowManager.hpp"

int main(int argc, char *argv[])
{
	try {
		WindowManager& wm = WindowManager::Instance();
		wm.Run();
		if (wm.NeedRestart()) {
			execvp(argv[0], argv);
		}
		return 0;
	} catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}
