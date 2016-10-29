#pragma once

extern "C" {
#include <xcb/xcb.h>
}

#include "Types.hpp"
#include "WindowManager.hpp"

class Workspace {
public:
    Workspace(std::string name, u32 id, xcb_rectangle_t* rectangle);
    xcb_window_t GetRootWindow() const;

private:
    xcb_window_t _workspaceRoot;
    xcb_window_t _root;
    std::string _name;
    u32 _id;
};

