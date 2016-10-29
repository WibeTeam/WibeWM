#include "WindowManager.hpp"
#include "config.hpp"

#include <stdexcept>


WindowManager& WindowManager::Instance() {
	static WindowManager instance;
	return instance;
}
void WindowManager::exit() {
	Instance()._exit = true;
}

void WindowManager::restart() {
	Instance()._needRestart = true;
	Instance()._exit = true;
}

void WindowManager::Run() {
	xcb_flush (_connection);
	while (GenericEventPtr event = xcb_wait_for_event(_connection)) {
		switch (event->response_type & ~0x80) {
			case XCB_EXPOSE: {
				printf("Expose event\n");
				ExposeEventPtr expose = (ExposeEventPtr) event;
				break;
			}
			case XCB_BUTTON_PRESS: {
				printf("Button press event\n");
				ButtonPressEventPtr buttonPress = (ButtonPressEventPtr)event;
				break;
			}
			case XCB_BUTTON_RELEASE: {
				printf("Button release event\n");
				ButtonReleaseEventPtr buttonRelease = (ButtonReleaseEventPtr)event;
				break;
			}
			case XCB_MOTION_NOTIFY: {
				printf("Motion notify event\n");
				MotionNotifyEventPtr MotionNotify = (MotionNotifyEventPtr)event;
				break;
			}
			case XCB_ENTER_NOTIFY: {
				printf("Enter notify event\n");
				EnterNotifyEventPtr ev = (EnterNotifyEventPtr)event;
				break;
			}
			case XCB_LEAVE_NOTIFY: {
				printf("Leave notify event\n");
				LeaveNotifyEventPtr ev = (LeaveNotifyEventPtr)event;
				break;
			}
			case XCB_KEY_PRESS: {
				printf("Key press event\n");
				KeyPressEventPtr ev = (KeyPressEventPtr)event;
				for (u32 i = 0; i < sizeof(hotkeys); ++i) {
					if (ev->detail == hotkeys[i].key && ev->state == hotkeys[i].mode) {
						hotkeys[i].func();
						break;
					}
				}
				break;
			}
			case XCB_KEY_RELEASE: {
				printf("Key press event\n");
				KeyReleaseEventPtr ev = (KeyReleaseEventPtr)event;
				break;
			}
			default:
				printf("Unknown event: %d\n", event->response_type);
				break;
		}
		free(event);

		if (_exit)
			break;
	}
}

bool WindowManager::NeedRestart() const {
	return _needRestart;
}

WindowManager::~WindowManager() {
	xcb_disconnect(_connection);
}

WindowManager::WindowManager()
		:_needRestart(false)
		,_exit(false)
		,_layout(std::make_unique<TilingLayout>()) {
	_connection = xcb_connect(nullptr, nullptr);
	if (xcb_connection_has_error(_connection)) {
		throw std::logic_error("Couldn't open display");
	}
	_rootScreen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;
	u32 mask = XCB_CW_EVENT_MASK;
	u32 values[] = { XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
	                 XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
	                 XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
	                 XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE };
	xcb_change_window_attributes(_connection, _rootScreen->root, mask, values);
}
