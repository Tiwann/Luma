#pragma once
#include "Rect.h"
#include "Vector.h"

namespace Luma
{
    template <NumberType T>
    struct TRect<T, 3>
    {
        T x, y, z;
        T width, height, depth;

        constexpr TRect(): x(T(0)), y(T(0)), z(T(0)),width(T(0)), height(T(0)), depth(T(0)){}
        constexpr TRect(T x, T y, T z, T width, T height, T depth): x(x), y(y), z(z),width(width), height(height), depth(depth){}

        constexpr T left() const { return x; }
        constexpr T right() const { return x + width; }
        constexpr T top() const { return y; }
        constexpr T bottom() const { return y + height; }
        constexpr T front() const { return z; }
        constexpr T back() const { return z + depth; }

        constexpr T volume() const { return width * height * depth; }
        constexpr bool empty() const { return width <= T(0) || height <= T(0) || depth <= T(0); }
        constexpr T centerX() const { return x + width / T(2); }
        constexpr T centerY() const { return y + height / T(2); }
        constexpr T centerZ() const { return z + depth / T(2); }

        constexpr TVector<T, 3> center() const { return {centerX(), centerY(), centerZ()}; }

        constexpr bool contains(T px, T py, T pz) const
        {
            return px >= left() && px <= right() &&
                py >= top() && py <= bottom() &&
                pz >= front() && pz <= back();
        }

        constexpr bool contains(const TRect& other) const
        {
            return other.left() >= left() &&
                other.right() <= right() &&
                other.top() >= top() &&
                other.bottom() <= bottom() &&
                other.front() >= front() &&
                other.back() <= back();
        }

        constexpr bool contains(const TVector<T, 3>& p) const
        {
            return contains(p.x, p.y, p.z);
        }

        constexpr bool intersects(const TRect& other) const
        {
            return !(right() < other.left() ||
                left() > other.right() ||
                bottom() < other.top() ||
                top() > other.bottom() ||
                back() < other.front() ||
                front() > other.back());
        }

        constexpr TRect intersection(const TRect& other) const
        {
            T nx = (left() > other.left()) ? left() : other.left();
            T ny = (top() > other.top()) ? top() : other.top();
            T nz = (front() > other.front()) ? front() : other.front();

            T nr = (right() < other.right()) ? right() : other.right();
            T nb = (bottom() < other.bottom()) ? bottom() : other.bottom();
            T nk = (back() < other.back()) ? back() : other.back();

            if (nr < nx || nb < ny || nk < nz)
                return TRect();

            return TRect(nx, ny, nz, nr - nx, nb - ny, nk - nz);
        }

        constexpr TRect unite(const TRect& other) const
        {
            T nx = (left() < other.left()) ? left() : other.left();
            T ny = (top() < other.top()) ? top() : other.top();
            T nz = (front() < other.front()) ? front() : other.front();

            T nr = (right() > other.right()) ? right() : other.right();
            T nb = (bottom() > other.bottom()) ? bottom() : other.bottom();
            T nk = (back() > other.back()) ? back() : other.back();

            return TRect(nx, ny, nz, nr - nx, nb - ny, nk - nz);
        }

        constexpr TRect translated(T dx, T dy, T dz) const
        {
            return TRect(x + dx, y + dy, z + dz, width, height, depth);
        }

        constexpr TRect translated(const TVector<T, 3>& t) const
        {
            return translated(t.x, t.y, t.z);
        }

        constexpr TRect scaled(T sx, T sy, T sz) const
        {
            return TRect(x * sx, y * sy, z * sz, width * sx, height * sy, depth * sz);
        }

        constexpr TRect scaled(const TVector<T, 3>& s) const
        {
            return scaled(s.x, s.y, s.z);
        }

        constexpr TRect inflated(T amount) const
        {
            return TRect(x - amount, y - amount, z - amount,
                         width + amount * T(2),
                         height + amount * T(2),
                         depth + amount * T(2));
        }

        constexpr void clampPoint(T& px, T& py, T& pz) const
        {
            if (px < left()) px = left();
            if (px > right()) px = right();

            if (py < top()) py = top();
            if (py > bottom())py = bottom();

            if (pz < front()) pz = front();
            if (pz > back()) pz = back();
        }

        constexpr void clampPoint(TVector<T, 3>& p) const
        {
            clampPoint(p.x, p.y, p.z);
        }

        constexpr TRect normalized() const
        {
            T nx = x, ny = y, nz = z;
            T nw = width, nh = height, nd = depth;

            if (nw < T(0))
            {
                nx += nw;
                nw = -nw;
            }
            if (nh < T(0))
            {
                ny += nh;
                nh = -nh;
            }
            if (nd < T(0))
            {
                nz += nd;
                nd = -nd;
            }

            return TRect(nx, ny, nz, nw, nh, nd);
        }

        template <NumberType U>
        constexpr TRect<U, 3> as() const
        {
            return TRect<U, 3>((U)x, (U)y, (U)z, (U)width, (U)height, (U)depth);
        }
    };
}
