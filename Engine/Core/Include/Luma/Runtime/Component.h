#pragma once
#include "Luma/Memory/RefCounted.h"
#include "Luma/Rendering/Camera.h"

namespace Luma
{
    struct ICommandBuffer;
    class FEntity;

    struct IComponent : IRefCounted<IComponent>
    {
        ~IComponent() override = default;

        virtual void onInit(){}
        virtual void onDestroy(){}
        virtual void onUpdate(double deltaTime){}
        virtual void onPhysicsUpdate(double deltaTime){}
        virtual void onPreRender(ICommandBuffer* cmdBuffer){}
        virtual void onRender(ICommandBuffer* cmdBuffer, const FCamera& camera){}
        virtual void onPostRender(){}

        void destroy(){ onDestroy(); }

        FEntity getOwner();
    };
}
