#pragma once


namespace Luma
{
    struct IInputProvider
    {
        virtual ~IInputProvider() = default;

        virtual bool initialize() = 0;
        virtual void destroy() = 0;

        
    };
}