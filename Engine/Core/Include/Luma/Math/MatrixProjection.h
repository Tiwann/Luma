#pragma once
#include "Matrix.h"

namespace Luma
{
    template<FloatType T>
    TMatrix<T, 4, 4> perspective(T fov, T aspectRatio, T near, T far);

    template<FloatType T>
    TMatrix<T, 4, 4> orthoCentered(T width, T height, T scale, T near, T far);
    
    template<FloatType T>
    TMatrix<T, 4, 4> orthoTopLeft(T width, T height, T scale, T near, T far);
}