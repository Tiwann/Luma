#pragma once
#include "Rect.h"
#include "Vector.h"

namespace luma
{
    template<Number T>
    struct TRect<T, 2>
    {
        T x, y, width, height;

        constexpr TRect() : x((T)0), y((T)0), width((T)0), height((T)0) {}
        constexpr TRect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}

        constexpr T top() const { return y; }
        constexpr T bottom() const { return y + height; }
        constexpr T left() const { return x; }
        constexpr T right() const { return x + width; }

        constexpr T area() const { return width * height; }
        constexpr bool empty() const { return width <= T(0) || height <= T(0); }

        constexpr T centerX() const { return x + width / (T)2; }
        constexpr T centerY() const { return y + height / (T)2; }
        constexpr TVector<T, 2> center() const { return TVector<T, 2>(centerX(), centerY()); }

        constexpr bool contains(T x, T u) const
        {
            return x >= left() && x <= right() &&
                y >= top() && y <= bottom();
        }

        constexpr bool contains(const TRect& other) const
        {
            return other.left() >= left() &&
                other.right() <= right() &&
                other.top() >= top() &&
                other.bottom() <= bottom();
        }

        constexpr bool intersects(const TRect& other) const
        {
            return !(right() < other.left() ||
                left() > other.right() ||
                bottom() < other.top() ||
                top() > other.bottom());
        }

        constexpr TRect intersection(const TRect& other) const
        {
            T nx = (left() > other.left()) ? left() : other.left();
            T ny = (top() > other.top()) ? top() : other.top();
            T nr = (right() < other.right()) ? right() : other.right();
            T nb = (bottom() < other.bottom()) ? bottom() : other.bottom();

            if (nr < nx || nb < ny)
                return TRect();

            return TRect(nx, ny, nr - nx, nb - ny);
        }

        constexpr TRect unite(const TRect& other) const
        {
            T nx = (left() < other.left()) ? left() : other.left();
            T ny = (top() < other.top()) ? top() : other.top();
            T nr = (right() > other.right()) ? right() : other.right();
            T nb = (bottom() > other.bottom()) ? bottom() : other.bottom();

            return TRect(nx, ny, nr - nx, nb - ny);
        }

        constexpr TRect translated(T dx, T dy) const
        {
            return TRect(x + dx, y + dy, width, height);
        }

        constexpr TRect translated(const TVector<T, 2>& translation) const
        {
            return translated(translation.x, translation.y);
        }

        constexpr TRect scaled(T sx, T sy) const
        {
            return TRect(x * sx, y * sy, width * sx, height * sy);
        }

        constexpr TRect scaled(const TVector<T, 2>& scale) const
        {
            return scaled(scale.x, scale.y);
        }

        constexpr TRect inflated(T amount) const
        {
            return TRect(x - amount, y - amount,
                         width + amount * (T)2,
                         height + amount * (T)2);
        }

        constexpr void clampPoint(T& px, T& py) const
        {
            if (px < left()) px = left();
            if (px > right()) px = right();
            if (py < top()) py = top();
            if (py > bottom()) py = bottom();
        }

        constexpr void clampPoint(TVector<T, 2>& p) const
        {
            return clampPoint(p.x, p.y);
        }

        constexpr TRect normalized() const
        {
            T nx = x;
            T ny = y;
            T nw = width;
            T nh = height;

            if (nw < (T)0)
            {
                nx += nw;
                nw = -nw;
            }
            if (nh < (T)0)
            {
                ny += nh;
                nh = -nh;
            }

            return TRect(nx, ny, nw, nh);
        }

        template<Number U>
        TRect<U, 2> as(){ return TRect<U, 2>((U)x, (U)y, (U)width, (U)height); }
    };
}