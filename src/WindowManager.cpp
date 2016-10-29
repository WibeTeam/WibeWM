#include "WindowManager.hpp"
#include "config.hpp"

#include <stdexcept>
#include <unistd.h>


WindowManager& WindowManager::Instance() {
	static WindowManager instance;
	return instance;
}
void WindowManager::quit(const Arg* args) {
	Instance()._exit = true;
}

void WindowManager::restart(const Arg* args) {
	Instance()._needRestart = true;
	Instance()._exit = true;
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
	xcb_flush (_connection);
	while (GenericEventPtr event = xcb_wait_for_event(_connection)) {
		switch (event->response_type & ~0x80) {
			case XCB_CREATE_NOTIFY: {
				printf("Notify created");
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
						hotkeys[i].func(&hotkeys[i].args);
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
	u32 values[] = { XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY };
	xcb_generic_error_t* e = xcb_request_check(_connection, xcb_change_window_attributes_checked(_connection, _rootScreen->root, mask, values));
	if (e != NULL) {
		xcb_disconnect(_connection);
		printf("Another window manager is already running");
		exit(EXIT_FAILURE);
	}
}
