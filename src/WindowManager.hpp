#pragma once

#include <xcb/xcb.h>


class WindowManager {
public:
	static const WindowManager& Instance();

	xcb_connection_t* GetConnection();

	~WindowManager();

private:        
	WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator=(const WindowManager&) = delete;


	xcb_connection_t* _connection;
	xcb_screen_t*     _screen;
};
