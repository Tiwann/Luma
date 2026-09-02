#pragma once
#include "ShaderStage.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/BufferView.h"
#include "Luma/Containers/StringView.h"
#include "Luma/Memory/RefCounted.h"

namespace Luma
{
    struct IRenderDevice;
    struct IBindingSet;

    struct IShaderProgram : IRefCounted<IShaderProgram>
    {
        IShaderProgram() = default;
        ~IShaderProgram() override = default;

        virtual bool initialize(IRenderDevice* device) = 0;
        virtual void destroy() = 0;
        virtual IBindingSet* createBindingSet(uint32_t setIndex) const = 0;


    protected:
        FShaderStageFlags m_Stages = EShaderStageBits::None;
    };
}
