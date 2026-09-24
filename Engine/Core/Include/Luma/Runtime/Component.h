#pragma once
#include "Luma/Runtime/Object.h"

namespace Luma
{
    class TransformComponent;
    struct CommandBuffer;
    class Entity;

    struct Component : Object
    {
        ~Component() override = default;

        Entity* getOwner() const;
        void setActive(bool active);
        bool isActive() const;

        TransformComponent* getTransformComponent() const;
    protected:
        virtual void onInit(){}
        virtual void onDestroy(){}
        virtual void onUpdate(double deltaTime){}
        virtual void onPhysicsUpdate(double deltaTime){}
        virtual void onLateUpdate(double deltaTime){}
        virtual void onRender(CommandBuffer* cmdBuffer){}

    private:
        friend class Entity;
        Entity* m_Owner = nullptr;
        bool m_Active = true;

        void initialize() { onInit(); }
        void destroy() override { onDestroy(); }
    };
}
