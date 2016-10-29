#include "Window.hpp"

Window::Window(const Window& parent, xcb_rectangle_t* rect, u32 mask, u32* values) {
	_window = xcb_generate_id(Connection::Instance().Get());
	xcb_create_window(
		Connection::Instance().Get(),
		XCB_COPY_FROM_PARENT, // ???
		_window,
		parent._window,
		rect->x,
		rect->y,
		rect->width,
		rect->height,
		XCB_WINDOW_CLASS_INPUT_ONLY, // ???
		0, // border_width ???
		XCB_COPY_FROM_PARENT, // ???
		mask,
		values
				);
}

void Window::Configure(u32 mask, u32* values) {
	xcb_change_window_attributes(Connection::Instance().Get(), _window, mask, values);
}

void Window::ChangeAttributes(u32 mask, u32* values) const {
	xcb_change_window_attributes(Connection::Instance().Get(), _window, mask, values);
}

void Window::Show() {
	xcb_map_window(Connection::Instance().Get(), _window);
}

void Window::Hide() {
    xcb_unmap_window(Connection::Instance().Get(), _window);
}

void Window::Resize(u32 width, u32 height) {
	u32 values[] = {width, height};
	xcb_configure_window(Connection::Instance().Get(), _window, (XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT), values);
}

void Window::Move(u32 x, u32 y) {
	u32 values[] = {x, y};
	xcb_configure_window(Connection::Instance().Get(), _window, (XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y), values);
}
