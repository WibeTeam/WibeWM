#pragma once

#include "Connection.hpp"
#include "Types.hpp"
#include "Window.hpp"

class Workspace {
public:
    Workspace(const Window& root, std::string name, xcb_rectangle_t* rectangle);
    const Window& GetRootWindow() const;

private:
    Window*           _root;
    std::string       _name;
    u32               _id;
};

