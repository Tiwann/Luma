#pragma once

namespace luma
{
    struct IBase
    {
        virtual ~IBase() {}
        virtual void destroy() = 0;
    };

    template<typename FDesc>
    struct TBase : IBase
    {
        ~TBase() override {}
        virtual bool initialize(const FDesc& desc) = 0;
    };
}