#pragma once
#include "Luma/Runtime/Object.h"

namespace Luma
{
    class FTransformComponent;
    struct CommandBuffer;
    class FEntity;

    struct IComponent : Object
    {
        ~IComponent() override = default;

        FEntity* getOwner() const;
        void setActive(bool active);
        bool isActive() const;

        FTransformComponent* getTransformComponent() const;
    protected:
        virtual void onInit(){}
        virtual void onDestroy(){}
        virtual void onUpdate(double deltaTime){}
        virtual void onPhysicsUpdate(double deltaTime){}
        virtual void onLateUpdate(double deltaTime){}
        virtual void onRender(CommandBuffer* cmdBuffer){}
    private:
        friend class FEntity;
        FEntity* m_Owner = nullptr;
        bool m_Active = true;

        void initialize() { onInit(); }
        void destroy() override { onDestroy(); }
    };
}
