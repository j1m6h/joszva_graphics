#include <memory>

#include "../../inc/base/input_impl.h"
#include "../../inc/base/vulkan_impl.h"
#include "../../inc/base/window.h"

#include "../../vendor/joszva_log/inc/logger.h"

using joszva::graphics::vector2;
using joszva::graphics::window;

using joszva::graphics::priv::window_impl;

namespace
{
    auto _logger = std::make_shared<joszva::logger>();
}

window::window(const std::string& title, int width, int height)
    : impl(new window_impl(title, width, height))
{
    _logger->info("Window created successfully");
}

window::~window()
{
}

void window::set_visible(bool visible) const
{
    impl->set_visible(visible);

    if (visible)
        _logger->info("Set window visible");
    else
        _logger->info("Set window invisible");
}

const std::string& window::get_title() const
{
    return impl->get_title();
}

void window::set_title(const std::string& title)
{
    impl->set_title(title);
    _logger->info("Set window title to [" + title + "]");
}

vector2<int> window::get_size() const 
{
    return impl->get_size();
}

void window::set_size(const vector2<int>& size)
{
    impl->set_size(size);
    _logger->info("New window size : " + std::to_string(size.x) + ", " + std::to_string(size.y));
}

const vector2<int> window::get_cursor_coords() const 
{
    return priv::input_impl::get_cursor_coords(*impl);
}

void window::set_cursor_coords(const vector2<int>& coord) const 
{
    return priv::input_impl::set_cursor_coords(*impl, coord);
    _logger->info("Set cursor coords : " + std::to_string(coord.x) + ", " + std::to_string(coord.y));
}

void window::destroy()
{
    delete impl;
    impl = nullptr;
}

const char** window::get_required_instance_extensions(uint32_t* count)
{
    return priv::vulkan_impl::get_required_instance_extensions(count);
}

VkResult window::create_vulkan_surface(const VkInstance& instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR& surface)
{
    return priv::vulkan_impl::create_vulkan_surface(*impl, instance, allocator, surface);
}

bool window::is_open()
{
    if (impl != nullptr)
    {
        return true;
    }

    return false;
}

void window::process_events()
{
    impl->process_events();

    static int it = 0;
    if (it == 0)
        _logger->info("Event processing started...");
    
    ++it;
}