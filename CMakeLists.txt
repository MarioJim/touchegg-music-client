cmake_minimum_required(VERSION 3.14)
project(touchegg_music_client VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

include(cmake/touchegg-dependency.cmake)

list(APPEND CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake/modules"
    "${touchegg_SOURCE_DIR}/cmake/Modules")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    include(cmake/code-style.cmake)
endif()

find_package(Threads REQUIRED) # Required to use std threads
find_package(LIBUDEV REQUIRED)
find_package(LIBINPUT REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(CAIRO REQUIRED)
find_package(X11 REQUIRED)
find_package(XTEST REQUIRED)
find_package(XRANDR REQUIRED)
find_package(XINPUT REQUIRED)
find_package(GLIB REQUIRED)
find_package(GTK3 REQUIRED)
find_package(LibPulse REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(APPEND SOURCE_FILES "${TOUCHEGG_SOURCE_FILES}")

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
        ${GTK3_INCLUDE_DIRS}
        ${LIBPULSE_INCLUDE_DIRS})
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
        ${GTK3_LIBRARIES}
        ${LIBPULSE_LIBRARIES})
add_definitions(-DAUTO_COLORS=ON)
