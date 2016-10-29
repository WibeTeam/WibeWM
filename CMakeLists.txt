cmake_minimum_required(VERSION 2.8)

project(wibe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
# TODO: Write normal CmakeList
list(APPEND SRC_LIST src/main.cpp)
list(APPEND SRC_LIST src/Workspace.cpp)
list(APPEND SRC_LIST src/Window.cpp)
list(APPEND SRC_LIST src/WindowManager.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} xcb)
