//
// Created by Junhao Wang (@Forkercat) on 2021/4/6.
//

#ifndef SRC_CORE_RAY_H_
#define SRC_CORE_RAY_H_

#include "geometry.h"

// Ray Declarations

class Ray
{
public:
    bool HasNaNs() const { return (origin.HasNaNs() || dir.HasNaNs()); }

    // Constructors
    Ray() : origin(0.f), dir(0.f), invDir(0.f) { }
    Ray(const Point3f& o, const Vector3f& d) : origin(o), dir(d)
    {
        invDir = Vector3f(1.f / d.x, 1.f / d.y, 1.f / d.z);
    }

    Point3f operator()(Float t) const { return origin + t * dir; }

    friend std::ostream& operator<<(std::ostream& os, const Ray& r)
    {
        os << "[origin=" << r.origin << ", dir=" << r.dir << "]";
        return os;
    }

    // Public Data
    Point3f  origin;
    Vector3f dir;
    Vector3f invDir;
};

#endif  // SRC_CORE_RAY_H_
