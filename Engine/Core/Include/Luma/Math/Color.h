#pragma once
#include "Concepts.h"

namespace Luma
{
    template<FloatType F>
    struct TColor
    {
        F r{F(0.0)}, g{F(0.0)}, b{F(0.0)}, a{F(1.0)};

        constexpr TColor() = default;
        constexpr TColor(F r, F g, F b, F a) : r{r}, g{g}, b{b}, a {a} {}
        constexpr explicit TColor(uint32_t hex)
            : r{static_cast<F>((hex >> 24) & 0xFF) / F(255.0)},
              g{static_cast<F>((hex >> 16) & 0xFF) / F(255.0)},
              b{static_cast<F>((hex >> 8) & 0xFF) / F(255.0)},
              a{static_cast<F>(hex & 0xFF) / F(255.0)} {}

        constexpr uint32_t toHex() const
        {
            static constexpr auto saturate = [] (F v) constexpr { return v < F(0.0) ? F(0.0) : v > F(1.0) ? F(1.0) : v; };
            const uint8_t red = static_cast<uint8_t>(saturate(r) * F(255.0) + F(0.5));
            const uint8_t green = static_cast<uint8_t>(saturate(g) * F(255.0) + F(0.5));
            const uint8_t blue = static_cast<uint8_t>(saturate(b) * F(255.0) + F(0.5));
            const uint8_t alpha = static_cast<uint8_t>(saturate(a) * F(255.0) + F(0.5));
            return (red << 24) | (green << 16) | (blue << 8) | (alpha);
        }
    };

    using FColor = TColor<float>;
}