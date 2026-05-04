#pragma once
#include "Luma/Runtime/Object.h"

namespace Luma
{
    struct ICommandBuffer;
    class FEntity;

    struct IComponent : IObject
    {
        ~IComponent() override = default;

        FEntity* getOwner() const;
        void setActive(bool active);
        bool isActive() const;

    protected:
        virtual void onInit(){}
        virtual void onDestroy(){}
        virtual void onUpdate(double deltaTime){}
        virtual void onPhysicsUpdate(double deltaTime){}
        virtual void onLateUpdate(double deltaTime){}
        virtual void onRender(ICommandBuffer* cmdBuffer){}
    private:
        friend class FEntity;
        FEntity* m_Owner = nullptr;
        bool m_Active = true;

        void initialize() { onInit(); }
        void destroy() override { onDestroy(); }
    };
}
