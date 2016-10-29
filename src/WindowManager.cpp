#include "WindowManager.hpp"

#include <stdexcept>


WindowManager& WindowManager::Instance() {
	static WindowManager instance;
	return instance;
}

void WindowManager::Run() {

        /* Create the window */
        xcb_window_t window    = xcb_generate_id (_connection);

        uint32_t     mask      = XCB_CW_EVENT_MASK;
        uint32_t     values[] = { XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
                                  XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                                  XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
                                  XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE };

        xcb_create_window (_connection,
                           0,                             /* depth               */
                           window,
                           _screen->root,                  /* parent window       */
                           0, 0,                          /* x, y                */
                           150, 150,                      /* width, height       */
                           10,                            /* border_width        */
                           XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                           _screen->root_visual,           /* visual              */
                           mask, values );                /* masks */

        /* Map the window on the screen */
        xcb_map_window (_connection, window);


	bool exitFlag = false;

	xcb_flush (_connection);
	while (GenericEventPtr event = xcb_wait_for_event(_connection)) {
		printf("event type: %x\n", event->response_type);
		switch (event->response_type & ~0x80) {
			case XCB_EXPOSE: {
				ExposeEventPtr expose = (ExposeEventPtr) event;
				break;
			}
			case XCB_BUTTON_PRESS: {
				ButtonPressEventPtr buttonPress = (ButtonPressEventPtr)event;
				if (buttonPress->detail == 24) {
					exitFlag = true;
				}
				break;
			}
			case XCB_BUTTON_RELEASE: {
				ButtonReleaseEventPtr buttonRelease = (ButtonReleaseEventPtr)event;
				break;
			}
			case XCB_MOTION_NOTIFY: {
				MotionNotifyEventPtr MotionNotify = (MotionNotifyEventPtr)event;
				break;
			}
			case XCB_ENTER_NOTIFY: {
				EnterNotifyEventPtr ev = (EnterNotifyEventPtr)event;
				break;
			}
			case XCB_LEAVE_NOTIFY: {
				LeaveNotifyEventPtr ev = (LeaveNotifyEventPtr)event;
				break;
			}
			case XCB_KEY_PRESS: {
				KeyPressEventPtr ev = (KeyPressEventPtr)event;
				printf("KeyPressEvent detail: %d\n", ev->detail);
				printf("KeyPressEvent state: %d\n", ev->state);
				if (ev->detail == 24) {
					exitFlag = false;
				}
				if (ev->detail == 99) {
					exitFlag = false;
				}
				break;
			}
			case XCB_KEY_RELEASE: {
				KeyReleaseEventPtr ev = (KeyReleaseEventPtr)event;
				break;
			}
			default:
				printf("Unknown event: %d\n", event->response_type);
				break;
		}
		free(event);

		if (exitFlag)
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
		:_needRestart(true) {
	_connection = xcb_connect(nullptr, nullptr);
	if (xcb_connection_has_error(_connection)) {
		throw std::logic_error("Couldn't open display");
	}
	_screen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;
}
