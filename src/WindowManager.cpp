#include "WindowManager.hpp"

#include <stdexcept>

const WindowManager& WindowManager::Instance() {
	static WindowManager instance;
	return instance;
}

xcb_connection_t* WindowManager::GetConnection() {
	return _connection;
}

WindowManager::~WindowManager() {
	xcb_disconnect(_connection);
}

WindowManager::WindowManager() {
	_connection = xcb_connect(nullptr, nullptr);
	if (xcb_connection_has_error(_connection)) {
		throw std::logic_error("Couldn't open display");
	}
	/* get the first screen */
	_screen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;
}
