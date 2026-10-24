#pragma once

namespace Luma
{
    struct Engine
    {
        virtual ~Engine() = default;

        virtual void initialize();
        virtual void start();
        virtual void update(float deltaTime);
        virtual void destroy();
    };
}