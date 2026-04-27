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

        TColor withOpacity(float opacity) {  return { r, g, b, opacity }; }
        TColor withRed(float red) { return { red, g, b, a }; }
        TColor withGreen(float green) { return { r, green, b, a }; }
        TColor withBlue(float blue) { return { r, g, blue, a }; }

        static const TColor Red;
        static const TColor Green;
        static const TColor Blue;
        static const TColor Magenta;
        static const TColor Cyan;
        static const TColor Yellow;
        static const TColor Black;
        static const TColor White;
        static const TColor Orange;
        static const TColor Pink;
        static const TColor Brown;
        static const TColor Purple;
        static const TColor Turquoise;
        static const TColor Grey;
    };
    
    template<FloatType F>
    const TColor<F> TColor<F>::Red        { 0xFF0000FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Green      { 0x00FF00FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Blue       { 0x0000FFFF };
    template<FloatType F>
    const TColor<F> TColor<F>::Magenta    { 0xFF00FFFF };
    template<FloatType F>
    const TColor<F> TColor<F>::Cyan       { 0x00FFFFFF };
    template<FloatType F>
    const TColor<F> TColor<F>::Yellow     { 0xFFFF00FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Black      { 0x000000FF };
    template<FloatType F>
    const TColor<F> TColor<F>::White      { 0xFFFFFFFF };
    template<FloatType F>
    const TColor<F> TColor<F>::Orange     { 0xFF8000FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Pink       { 0xFFC0CBFF };
    template<FloatType F>
    const TColor<F> TColor<F>::Brown      { 0x00000000 };
    template<FloatType F>
    const TColor<F> TColor<F>::Purple     { 0x800080FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Turquoise  { 0x40E0D0FF };
    template<FloatType F>
    const TColor<F> TColor<F>::Grey       { 0x808080FF };

    using FColor = TColor<float>;
}