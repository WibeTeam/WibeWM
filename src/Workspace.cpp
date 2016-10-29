// Created by tener on 10/30/16.
//

#include "Workspace.hpp"

Workspace::Workspace(const Window& root, std::string name, xcb_rectangle_t* rectangle):
    _name{name} {
    _id = xcb_generate_id(Connection::Instance().Get());

    if (rectangle) {
        u32 create_values[] = {XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW};
        
        _root = new Window(root, rectangle, XCB_CW_EVENT_MASK, create_values);
        u32 configure_values[] = {XCB_STACK_MODE_BELOW};
        _root->Configure(XCB_CONFIG_WINDOW_STACK_MODE, configure_values);
    }

    u32 values_off[] = {ROOT_MASK & ~XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY};
    u32 values_on[]  = {ROOT_MASK};
    
    root.ChangeAttributes(XCB_CW_EVENT_MASK, values_off); // TODO: move it to Window::Show()
    _root->Show();
    root.ChangeAttributes(XCB_CW_EVENT_MASK, values_on); // TODO: move it to Window::Show()
    xcb_flush(Connection::Instance().Get());
}

const Window& Workspace::GetRootWindow() const {
    return *_root;
}
