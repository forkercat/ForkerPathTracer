//
// Created by Junhao Wang (@Forkercat) on 2021/4/7.
//

#ifndef SRC_CORE_BOUNDS_H_
#define SRC_CORE_BOUNDS_H_

#include <array>

#include "constant.h"
#include "geometry.h"
#include "ray.h"

class Bounds3
{
public:
    // Constructors
    Bounds3() : pMin(MaxFloat), pMax(LowestFloat) { }
    explicit Bounds3(const Point3f& p) : pMin(p), pMax(p) { }
    Bounds3(const Point3f& p1, const Point3f& p2)
        : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
          pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z))
    {
    }

    inline const Vector3f& operator[](int i) const { return (i == 0) ? pMin : pMax; }
    inline Vector3f&       operator[](int i) { return (i == 0) ? pMin : pMax; }

    Vector3f Diagonal() const { return pMax - pMin; }
    int      MaxExtent() const { return MaxDimension(Diagonal()); }
    Vector3f Centroid() { return pMin * 0.5 + pMax * 0.5; }
    bool     IntersectP(const Ray& ray, const Vector3f& invDir,
                        const std::array<int, 3>& dirIsNeg, Float tMax) const;

    friend std::ostream& operator<<(std::ostream& os, const Bounds3& bounds)
    {
        os << "[ " << bounds.pMin << " - " << bounds.pMax << " ]";
        return os;
    }

    Point3f pMin, pMax;
};

// Intersect
inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg, Float tMax) const
{
    const Bounds3& bounds = *this;
    // Check X & Y Slabs
    Float tEnter = (bounds[dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    Float tExit = (bounds[1 - dirIsNeg[0]].x - ray.origin.x) * invDir.x;

    Float tyMin = (bounds[dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    Float tyMax = (bounds[1 - dirIsNeg[1]].y - ray.origin.y) * invDir.y;

    if (tEnter > tyMax || tyMin > tExit) return false;  // no intersection
    if (tyMin > tEnter) tEnter = tyMin;                 // update tEnter
    if (tyMax < tExit) tExit = tyMax;                   // update tExit

    // Check Z Slab
    Float tzMin = (bounds[dirIsNeg[2]].z - ray.origin.z) * invDir.z;
    Float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.origin.z) * invDir.z;

    if (tEnter > tzMax || tzMin > tExit) return false;
    if (tzMin > tEnter) tEnter = tzMin;
    if (tzMax < tExit) tExit = tzMax;

    return (tEnter < tMax) && (tExit > 0);
}

// Bounds Inline Functions

inline Bounds3 Union(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Min(b1.pMin, b2.pMin);
    ret.pMax = Max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 Union(const Bounds3& b, const Vector3f& p)
{
    Bounds3 ret;
    ret.pMin = Min(b.pMin, p);
    ret.pMax = Max(b.pMax, p);
    return ret;
}

inline Bounds3 Intersect(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Max(b1.pMin, b2.pMin);
    ret.pMax = Min(b1.pMax, b2.pMax);
    return ret;
}

inline bool Overlaps(const Bounds3& b1, const Bounds3& b2)
{
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    return (x && y && z);
}

inline bool Inside(const Point3f& p, const Bounds3& b)
{
    return (p.x >= b.pMin.x && p.x <= b.pMax.x) && (p.y >= b.pMin.y && p.y <= b.pMax.y) &&
           (p.z >= b.pMin.z && p.z <= b.pMax.z);
}

inline bool InsideExclusive(const Point3f& p, const Bounds3& b)
{
    return (p.x >= b.pMin.x && p.x < b.pMax.x) && (p.y >= b.pMin.y && p.y < b.pMax.y) &&
           (p.z >= b.pMin.z && p.z < b.pMax.z);
}

inline Bounds3 Expand(const Bounds3& b, Float delta)
{
    return Bounds3(b.pMin - Vector3f(delta), b.pMax + Vector3f(delta));
}

#endif  // SRC_CORE_BOUNDS_H_
