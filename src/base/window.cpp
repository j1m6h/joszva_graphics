#include "../../inc/base/input_impl.h"
#include "../../inc/base/vulkan_impl.h"
#include "../../inc/base/window.h"

using joszva::engine::vector2;
using joszva::engine::window;

using joszva::engine::priv::window_impl;

window::window(const std::string& title, int width, int height)
    : impl(new window_impl(title, width, height))
{
}

window::~window()
{
}

void window::set_visible(bool visible) const
{
    impl->set_visible(visible);
}

const std::string& window::get_title() const
{
    return impl->get_title();
}

void window::set_title(const std::string& title)
{
    impl->set_title(title);
}

vector2<int> window::get_size() const 
{
    return impl->get_size();
}

void window::set_size(const vector2<int>& size)
{
    return impl->set_size(size);
}

const vector2<int> window::get_cursor_coords() const 
{
    return priv::input_impl::get_cursor_coords(*impl);
}

void window::set_cursor_coords(const vector2<int>& coord) const 
{
    return priv::input_impl::set_cursor_coords(*impl, coord);
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
}