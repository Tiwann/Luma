#pragma once
#include "Array.h"
#include "BufferView.h"

namespace Luma
{
    template <typename T>
class TArrayView : public TBufferView<T>
    {
    public:
        constexpr TArrayView() : TBufferView<T>()
        {
        }

        TArrayView(const TArray<T>& array) : TBufferView<T>(array.data(), array.count())
        {
        }

        TArrayView(const T& element) : TBufferView<T>(element){}
    };
}
