#pragma once
#include "AABB.h"
#include "Rect2.h"
#include <cmath>
#include <limits>

namespace Luma
{
    template <NumberType T>
    struct TAABB<T, 2> : TRect<T, 2>
    {
        using RectType = TRect<T, 2>;
        using RectType::x;
        using RectType::y;
        using RectType::width;
        using RectType::height;

        constexpr TAABB() = default;

        constexpr TAABB(T x, T y, T w, T h) : RectType(x, y, w, h){}
        
        constexpr TVector<T, 2> min() const
        {
            return {this->left(), this->top()};
        }

        constexpr TVector<T, 2> max() const
        {
            return {this->right(), this->bottom()};
        }
        
        static constexpr TAABB fromMinMax(const TVector<T, 2>& min, const TVector<T, 2>& max)
        {
            return TAABB(min.x, min.y, max.x - min.x, max.y - min.y);
        }
        
        constexpr TVector<T, 2> closestPoint(const TVector<T, 2>& p) const
        {
            TVector<T, 2> result = p;
            this->clampPoint(result);
            return result;
        }
        
        constexpr T distanceSq(const TVector<T, 2>& p) const
        {
            auto c = closestPoint(p);
            T dx = p.x - c.x;
            T dy = p.y - c.y;
            return dx * dx + dy * dy;
        }

        constexpr T distance(const TVector<T, 2>& p) const
        {
            return std::sqrt(distanceSq(p));
        }
        
        constexpr TVector<T, 2> penetration(const TAABB& other) const
        {
            constexpr auto abs = [](T a) constexpr { return a < T(0) ? -a : a; };
            const T dx1 = other.right() - this->left();
            const T dx2 = this->right() - other.left();
            const T dy1 = other.bottom() - this->top();
            const T dy2 = this->bottom() - other.top();

            if (dx1 <= T(0) || dx2 <= T(0) || dy1 <= T(0) || dy2 <= T(0))
                return {T(0), T(0)};

            const T px = (dx1 < dx2) ? dx1 : -dx2;
            const T py = (dy1 < dy2) ? dy1 : -dy2;
            
            return abs(px) < abs(py) ? TVector<T, 2>{px, T(0)} : TVector<T, 2>{T(0), py};
        }
        
        constexpr void expand(const TVector<T, 2>& p)
        {
            T minX = (p.x < this->left()) ? p.x : this->left();
            T minY = (p.y < this->top()) ? p.y : this->top();
            T maxX = (p.x > this->right()) ? p.x : this->right();
            T maxY = (p.y > this->bottom()) ? p.y : this->bottom();

            this->x = minX;
            this->y = minY;
            this->width = maxX - minX;
            this->height = maxY - minY;
        }
        
        constexpr void expand(const TAABB& other)
        {
            expand(other.min());
            expand(other.max());
        }
        
        static constexpr TAABB merge(const TAABB& a, const TAABB& b)
        {
            T minX = (a.left() < b.left()) ? a.left() : b.left();
            T minY = (a.top() < b.top()) ? a.top() : b.top();
            T maxX = (a.right() > b.right()) ? a.right() : b.right();
            T maxY = (a.bottom() > b.bottom()) ? a.bottom() : b.bottom();

            return TAABB(minX, minY, maxX - minX, maxY - minY);
        }
        
        constexpr bool intersectsRay(
            const TVector<T, 2>& origin,
            const TVector<T, 2>& dir,
            T& tMin,
            T& tMax) const
        {
            constexpr auto min = [](T a, T b) constexpr { return a < b ? a : b; };
            constexpr auto max = [](T a, T b) constexpr { return a > b ? a : b; };
            
            const T t1 = (this->left() - origin.x) / dir.x;
            const T t2 = (this->right() - origin.x) / dir.x;
            const T t3 = (this->top() - origin.y) / dir.y;
            const T t4 = (this->bottom() - origin.y) / dir.y;

            const T tmin = max(min(t1, t2), min(t3, t4));
            const T tmax = min(max(t1, t2), max(t3, t4));

            if (tmax < T(0) || tmin > tmax)
                return false;

            tMin = tmin;
            tMax = tmax;
            return true;
        }

        constexpr bool intersects(const TAABB& other, T epsilon = std::numeric_limits<T>::epsilon()) const
        {
            return !(this->right() < other.left() - epsilon ||
                this->left() > other.right() + epsilon ||
                this->bottom() < other.top() - epsilon ||
                this->top() > other.bottom() + epsilon);
        }
    };
}
