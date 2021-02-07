cmake_minimum_required(VERSION 3.19)
project(touchegg_music_client VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        touchegg
        GIT_REPOSITORY https://github.com/JoseExposito/touchegg.git
        GIT_TAG 2.0.6
        GIT_SHALLOW ON
        PATCH_COMMAND patch --strip=1 --forward -i ${PROJECT_SOURCE_DIR}/touchegg_cmake_warnings.patch || true
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${touchegg_SOURCE_DIR}/cmake/Modules")
FetchContent_MakeAvailable(touchegg)


find_package(Threads REQUIRED) # Required to use std threads
find_package(Libudev REQUIRED)
find_package(Libinput REQUIRED)
find_package(PugiXML REQUIRED)
find_package(Cairo REQUIRED)
find_package(X11 REQUIRED)
find_package(XTest REQUIRED)
find_package(Xrandr REQUIRED)
find_package(XInput REQUIRED)
find_package(GLib REQUIRED)
find_package(Gtk3 REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${touchegg_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${touchegg_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES "${touchegg_SOURCE_DIR}/src/main.cpp")

add_executable(touchegg_music_client ${SOURCE_FILES})
target_include_directories(touchegg_music_client PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${touchegg_SOURCE_DIR}/src
        ${LIBUDEV_INCLUDE_DIRS}
        ${LIBINPUT_INCLUDE_DIRS}
        ${PUGIXML_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
        ${XTEST_INCLUDE_DIRS}
        ${XRANDR_INCLUDE_DIRS}
        ${XINPUT_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS})
target_link_libraries(touchegg_music_client
        stdc++fs # std::filesystem
        Threads::Threads # std::thread
        ${LIBUDEV_LIBRARIES}
        ${LIBINPUT_LIBRARIES}
        ${PUGIXML_LIBRARIES}
        ${CAIRO_LIBRARIES}
        ${X11_LIBRARIES}
        ${XTEST_LIBRARIES}
        ${XRANDR_LIBRARIES}
        ${XINPUT_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES})
add_definitions(-DAUTO_COLORS=ON)
