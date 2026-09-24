#pragma once
#include "Array.h"
#include "BufferView.h"

namespace Luma
{
    template <typename T>
    class ArrayView : public BufferView<T>
    {
    public:
        constexpr ArrayView() : BufferView<T>()
        {
        }

        ArrayView(const Array<T>& array) : BufferView<T>(array.data(), array.count())
        {
        }

        ArrayView(const T& element) : BufferView<T>(element){}
    };
}
