#pragma once
#include "Luma/Math/MatrixProjection.h"
#include "Luma/Math/Functions.h"
#include "Luma/Math/Matrix4.h"
#include "Luma/Math/Vector4.h"

namespace Luma
{
    template <FloatType F>
    TMatrix<F, 4, 4> perspective(F fov, F aspectRatio, F near, F far)
    {
        const F scale = F(1.0) / std::tan(radians(fov * 0.5));
        const F m22 = far / (near - far);
        const F m32 = -(far * near) / (far - near);

        return TMatrix<F, 4, 4> {
            FVector4<F>(scale / aspectRatio, F(0.0),  F(0.0),  F(0.0)),
            FVector4<F>(F(0.0),               scale,  F(0.0),  F(0.0)),
            FVector4<F>(F(0.0),               F(0.0),   m22,   F(-1.0)),
            FVector4<F>(F(0.0),               F(0.0),   m32,   F(0.0))
        };
    }

    template <FloatType F>
    TMatrix<F, 4, 4> orthoCentered(F width, F height, F scale, F near, F far)
    {
        const TMatrix<F, 4, 4> projection
        {
            FVector4<F>(scale * F(2.0)/ width, F(0.0), F(0.0), F(0.0)),
            FVector4<F>(F(0.0), scale * F(2.0)/ height, F(0.0), F(0.0)),
            FVector4<F>(F(0.0), F(0.0), F(-1.0) / (far - near), F(0.0)),
            FVector4<F>(F(0.0), F(0.0), -near / (far - near), F(1.0))
        };
        return projection;
    }

    template <FloatType F>
    TMatrix<F, 4, 4> orthoTopLeft(F width, F height, F scale, F near, F far)
    {
        const TMatrix<F, 4, 4> projection
        {
            FVector4<F>(scale * F(2.0) / width, F(0.0), F(0.0), F(0.0)),
            FVector4<F>(F(0.0), scale * F(2.0) / height, F(0.0), F(0.0)),
            FVector4<F>(F(0.0), F(0.0), F(-1.0) / (far - near), F(0.0)),
            FVector4<F>(F(-1.0), F(-1.0), -near / (far - near), F(1.0))
        };
        return projection;
    }

    template TMatrix<float, 4, 4> perspective(float, float, float, float);
    template TMatrix<double, 4, 4> perspective(double, double, double, double);
    template TMatrix<float, 4, 4> orthoCentered(float, float, float, float, float);
    template TMatrix<double, 4, 4> orthoCentered(double, double, double, double, double);
    template TMatrix<float, 4, 4> orthoTopLeft(float, float, float, float, float);
    template TMatrix<double, 4, 4> orthoTopLeft(double, double, double, double, double);
}
