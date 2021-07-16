cmake_minimum_required(VERSION 3.5)

project(joszva_engine)

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
    src/base/logger.cpp
    src/base/mouse.cpp
    src/base/window.cpp)

set(imgui_sources 
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_impl_vulkan.cpp 
    vendor/imgui/imgui_tables.cpp 
    vendor/imgui/imgui_widgets.cpp 
    vendor/imgui/imgui.cpp
    vendor/imgui/joszva_backend/imgui_impl_joszva.cpp)

add_library(${PROJECT_NAME} SHARED
    ${base_sources}
    ${platform_specific_sources}
    ${imgui_sources})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

if(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
endif()