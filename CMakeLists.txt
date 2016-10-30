cmake_minimum_required(VERSION 2.8)

project(wibe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(SRC_LIST
    src/Drawable.cpp
    src/WindowManager.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${FREETYPE_LIBRARIES})
