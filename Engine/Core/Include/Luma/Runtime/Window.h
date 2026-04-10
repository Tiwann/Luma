#pragma once
#include <cstdint>
#include <string>

namespace luma
{
    struct FWindowCreateInfo
    {
        std::string title;
        uint32_t width;
        uint32_t height;
        uint32_t flags;
    };


    struct IWindow
    {
        virtual ~IWindow() = default;
        virtual bool initialize(const FWindowCreateInfo& createInfo) = 0;
        virtual void destroy() = 0;
        virtual void pollEvents() = 0;
        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        struct Impl;
    };
}
