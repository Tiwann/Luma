#pragma once
#include "AABB.h"
#include "Rect3.h"
#include <cmath>
#include <limits>

namespace luma
{
    template <NumberType T>
    struct TAABB<T, 3> : TRect<T, 3>
    {
        using RectType = TRect<T, 3>;
        using RectType::x;
        using RectType::y;
        using RectType::z;
        using RectType::width;
        using RectType::height;
        using RectType::depth;

        constexpr TAABB() = default;
        constexpr TAABB(T x, T y, T z, T w, T h, T d) : RectType(x, y, z, w, h, d) {}

        constexpr TVector<T, 3> min() const
        {
            return { this->left(), this->top(), this->front() };
        }

        constexpr TVector<T, 3> max() const
        {
            return { this->right(), this->bottom(), this->back() };
        }

        static constexpr TAABB fromMinMax(const TVector<T, 3>& min, const TVector<T, 3>& max)
        {
            return TAABB(
                min.x, min.y, min.z,
                max.x - min.x,
                max.y - min.y,
                max.z - min.z
            );
        }

        constexpr TVector<T, 3> center() const
        {
            return {
                x + width  / T(2),
                y + height / T(2),
                z + depth  / T(2)
            };
        }

        constexpr TVector<T, 3> closestPoint(const TVector<T, 3>& p) const
        {
            TVector<T, 3> result = p;
            this->clampPoint(result);
            return result;
        }

        constexpr T distanceSq(const TVector<T, 3>& p) const
        {
            auto c = closestPoint(p);
            T dx = p.x - c.x;
            T dy = p.y - c.y;
            T dz = p.z - c.z;
            return dx * dx + dy * dy + dz * dz;
        }

        constexpr T distance(const TVector<T, 3>& p) const
        {
            return std::sqrt(distanceSq(p));
        }

        constexpr TVector<T, 3> penetration(const TAABB& other) const
        {
            constexpr auto abs = [](T n) constexpr { return n < 0 ? -n : n; };
            const T dx1 = other.right() - this->left();
            const T dx2 = this->right() - other.left();

            const T dy1 = other.bottom() - this->top();
            const T dy2 = this->bottom() - other.top();

            const T dz1 = other.back() - this->front();
            const T dz2 = this->back() - other.front();

            if (dx1 <= T(0) || dx2 <= T(0) ||
                dy1 <= T(0) || dy2 <= T(0) ||
                dz1 <= T(0) || dz2 <= T(0))
                return { T(0), T(0), T(0) };

            const T px = (dx1 < dx2) ? dx1 : -dx2;
            const T py = (dy1 < dy2) ? dy1 : -dy2;
            const T pz = (dz1 < dz2) ? dz1 : -dz2;

            const T ax = abs(px);
            const T ay = abs(py);
            const T az = abs(pz);

            if (ax < ay && ax < az) return { px, T(0), T(0) };
            if (ay < az)            return { T(0), py, T(0) };
            return { T(0), T(0), pz };
        }

        constexpr void expand(const TVector<T, 3>& p)
        {
            const T minX = (p.x < this->left())   ? p.x : this->left();
            const T minY = (p.y < this->top())    ? p.y : this->top();
            const T minZ = (p.z < this->front())  ? p.z : this->front();

            const T maxX = (p.x > this->right())  ? p.x : this->right();
            const T maxY = (p.y > this->bottom()) ? p.y : this->bottom();
            const T maxZ = (p.z > this->back())   ? p.z : this->back();

            x = minX;
            y = minY;
            z = minZ;

            width  = maxX - minX;
            height = maxY - minY;
            depth  = maxZ - minZ;
        }

        constexpr void expand(const TAABB& other)
        {
            expand(other.min());
            expand(other.max());
        }

        static constexpr TAABB merge(const TAABB& a, const TAABB& b)
        {
            const T minX = (a.left()   < b.left())   ? a.left()   : b.left();
            const T minY = (a.top()    < b.top())    ? a.top()    : b.top();
            const T minZ = (a.front()  < b.front())  ? a.front()  : b.front();

            const T maxX = (a.right()  > b.right())  ? a.right()  : b.right();
            const T maxY = (a.bottom() > b.bottom()) ? a.bottom() : b.bottom();
            const T maxZ = (a.back()   > b.back())   ? a.back()   : b.back();

            return TAABB(
                minX, minY, minZ,
                maxX - minX,
                maxY - minY,
                maxZ - minZ
            );
        }

        constexpr bool intersectsRay(
            const TVector<T, 3>& origin,
            const TVector<T, 3>& dir,
            T& tMin,
            T& tMax) const
        {
            constexpr auto min = [](const T& a, const T& b) constexpr -> const T& { return a < b ? a : b; };
            constexpr auto max = [](const T& a, const T& b) constexpr -> const T& { return a > b ? a : b; };
            
            const T t1 = (this->left()   - origin.x) / dir.x;
            const T t2 = (this->right()  - origin.x) / dir.x;
            const T t3 = (this->top()    - origin.y) / dir.y;
            const T t4 = (this->bottom() - origin.y) / dir.y;
            const T t5 = (this->front()  - origin.z) / dir.z;
            const T t6 = (this->back()   - origin.z) / dir.z;

            const T& tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
            const T& tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

            if (tmax < T(0) || tmin > tmax)
                return false;

            tMin = tmin;
            tMax = tmax;
            return true;
        }

        constexpr bool intersects(const TAABB& other, T epsilon) const
        {
            return !(this->right()  < other.left()   - epsilon ||
                     this->left()   > other.right()  + epsilon ||
                     this->bottom() < other.top()    - epsilon ||
                     this->top()    > other.bottom() + epsilon ||
                     this->back()   < other.front()  - epsilon ||
                     this->front()  > other.back()   + epsilon);
        }
    };
}