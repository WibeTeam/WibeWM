#pragma once

#include "Connection.hpp"
#include "Types.hpp"

class Window
{
public:
	Window(const Window& parent, xcb_rectangle_t *rect, u32 mask, u32 *values);
	Window(xcb_window_t window):_window(window) {}
	
	void Configure(u32 mask, u32* values);
	void ChangeAttributes(u32 mask, u32* values) const;
	
	void Resize(u32 width, u32 height);
	void Move(u32 x, u32 y);
	
	void Show();
    void Hide();

	xcb_window_t Get() {
		return _window;
	}

private:
	xcb_window_t       _window;
};
