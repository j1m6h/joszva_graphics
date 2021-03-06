cmake_minimum_required(VERSION 3.5)

project(joszva_graphics)

if(UNIX)
    set(platform_specific_sources
        src/native/x11/display_x11
        src/native/x11/input_impl_x11.cpp
        src/native/x11/vulkan_impl_x11.cpp
        src/native/x11/window_impl_x11.cpp)
endif()

if(WIN32)
    set(platform_specific_sources
        src/native/win32/input_impl_win32.cpp
        src/native/win32/vulkan_impl_win32.cpp
        src/native/win32/window_impl_win32.cpp)
endif()

set(base_sources
    src/base/color.cpp
    src/base/keyboard.cpp
    src/base/mouse.cpp
    src/base/window.cpp)

add_library(${PROJECT_NAME} SHARED
    ${platform_specific_sources}
    ${base_sources})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

target_link_libraries(${PROJECT_NAME} "/home/joshua/Desktop/joszva_engine/vendor/joszva_log/lib/libjoszva_log.so")

if(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
endif()