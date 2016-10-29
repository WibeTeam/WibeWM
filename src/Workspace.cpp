// Created by tener on 10/30/16.
//

#include "Workspace.hpp"

Workspace::Workspace(std::string name, u32 id, xcb_rectangle_t* rectangle):
    _name{name},
    _id{id} {
    auto connection = WindowManager::Instance().GetConnection();
    auto root_window = WindowManager::Instance().GetRootWindow();
    _workspaceRoot = xcb_generate_id(connection);

    if (rectangle) {
        _root = xcb_generate_id(connection);
        u32 create_values[] = {XCB_EVENT_MASK_ENTER_WINDOW};
        xcb_create_window(connection, XCB_COPY_FROM_PARENT, _root, root_window,
                          rectangle->x, rectangle->y, rectangle->width, rectangle->height, 0,
                          XCB_WINDOW_CLASS_INPUT_ONLY, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, create_values);
        u32 configure_values[] = {XCB_STACK_MODE_BELOW};
        xcb_configure_window(connection, _root, XCB_CONFIG_WINDOW_STACK_MODE, configure_values);
    }

    u32 values_off[] = {(XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY) & ~XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
    u32 values_on[]  = {(XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY)};
    xcb_change_window_attributes(connection, root_window, XCB_CW_EVENT_MASK, values_off);
    xcb_map_window(connection, _workspaceRoot);
    xcb_change_window_attributes(connection, root_window, XCB_CW_EVENT_MASK, values_on);
    xcb_flush(connection);
}

xcb_window_t Workspace::GetRootWindow() const {
    return _workspaceRoot;
}