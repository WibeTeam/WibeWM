#include "WindowManager.hpp"
#include "config.hpp"
#include "Layouts.hpp"

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
	xcb_flush (Connection::Instance().Get());
	while (GenericEventPtr event = xcb_wait_for_event(Connection::Instance().Get())) {
		switch (event->response_type & ~0x80) {
			case XCB_KEY_PRESS: {
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
				KeyReleaseEventPtr ev = (KeyReleaseEventPtr)event;
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
			case XCB_EXPOSE: {
				printf("Expose event\n");
				ExposeEventPtr expose = (ExposeEventPtr)event;
				break;
			} /*
			case XCB_UNMAP_NOTIFY: {
				xcb_unmap_notify_event_t* ev = (xcb_unmap_notify_event_t*)event;
				Window root = Window(_rootScreen->root);
				u32 configure_values[] = {XCB_STACK_MODE_BELOW};
				root.Configure(XCB_CONFIG_WINDOW_STACK_MODE, configure_values);
				u32 values_off[] = {ROOT_VALUES & ~XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
				u32 values_on[]  = {ROOT_VALUES};

				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_off); // TODO: move it to Window::Show()

				Window window = ev->window;

				u32 x[] = {XCB_EVENT_MASK_KEY_PRESS};
				window.Configure(XCB_CW_EVENT_MASK, x);
				_layout->RemoveWindow(window);


				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_on); // TODO: move it to Window::Show()
				xcb_flush(Connection::Instance().Get());
				break;
			}*/
			case XCB_DESTROY_NOTIFY: {
				xcb_destroy_notify_event_t* ev = (xcb_destroy_notify_event_t*)event;
				Window root = Window(_rootScreen->root);
				u32 configure_values[] = {XCB_STACK_MODE_BELOW};
				root.Configure(XCB_CONFIG_WINDOW_STACK_MODE, configure_values);
				u32 values_off[] = {ROOT_VALUES & ~XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
				u32 values_on[]  = {ROOT_VALUES};

				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_off); // TODO: move it to Window::Show()

				Window window = ev->window;
				u32 x[] = {XCB_EVENT_MASK_KEY_PRESS};

				window.Configure(XCB_CW_EVENT_MASK, x);
				_layout->RemoveWindow(window);


				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_on); // TODO: move it to Window::Show()
				xcb_flush(Connection::Instance().Get());
				break;
			}
			case XCB_MAP_REQUEST: {
				xcb_map_request_event_t* ev = (xcb_map_request_event_t*)event;
				printf("\nGot MAP REQUEST!\nparent: %d\nwindow: %d\n", ev->parent, ev->window);
				Window root = Window(_rootScreen->root);
				u32 configure_values[] = {XCB_STACK_MODE_BELOW};
				root.Configure(XCB_CONFIG_WINDOW_STACK_MODE, configure_values);
				u32 values_off[] = {ROOT_VALUES & ~XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
				u32 values_on[]  = {ROOT_VALUES};

				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_off); // TODO: move it to Window::Show()
				Window window = ev->window;
				u32 x[] = {XCB_EVENT_MASK_KEY_PRESS};
				window.Configure(XCB_CW_EVENT_MASK, x);
				_layout->AddWindow(window);
				root.ChangeAttributes(XCB_CW_EVENT_MASK, values_on); // TODO: move it to Window::Show()
				xcb_flush(Connection::Instance().Get());
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

Window WindowManager::GetRootWindow() const {
	return _rootScreen->root;
}

WindowManager::~WindowManager() {
	xcb_disconnect(Connection::Instance().Get());
}

WindowManager::WindowManager()
		:_needRestart(false)
		,_exit(false){
		
	_rootScreen = xcb_setup_roots_iterator(xcb_get_setup(Connection::Instance().Get())).data;
	u32 mask = XCB_CW_EVENT_MASK;
	u32 root_values[] = {ROOT_VALUES};
	if (xcb_request_check(Connection::Instance().Get(), xcb_change_window_attributes_checked(Connection::Instance().Get(), _rootScreen->root, mask, root_values))) {
		throw std::logic_error("Another window manager is already running");
	}
	xcb_rectangle_t rect = {0, 0, _rootScreen->width_in_pixels, _rootScreen->height_in_pixels};
//	_currentWorkspace = new Workspace(_rootScreen->root, "Workspace1", &rect);
	_layout = new TilingLayout(_rootScreen->width_in_pixels, _rootScreen->height_in_pixels);
}
