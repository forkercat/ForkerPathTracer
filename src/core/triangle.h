//
// Created by Junhao Wang (@Forkercat) on 2021/4/8.
//

#ifndef SRC_CORE_TRIANGLE_H_
#define SRC_CORE_TRIANGLE_H_

#include "common.h"
#include "hittable.h"

// Triangle Definitions
class Triangle : public Hittable
{
public:
    // Constructors
    Triangle(Point3f v0, Point3f v1, Point3f v2) : v0(v0), v1(v1), v2(v2)
    {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = Normalize(Cross(e1, e2));
    }

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;

    // Public Data
    Point3f  v0, v1, v2;
    Vector3f e1, e2;
    Vector2f t0, t1, t2;  // texture coords
    Vector3f normal;
    // Material
};

bool Triangle::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    return false;
}

Bounds3 Triangle::WorldBound() const
{
    return Union(Bounds3(v0, v1), v2);
}

/////////////////////////////////////////////////////////////////////////////////

// MeshTriangle Definitions
class MeshTriangle : public Hittable
{

};

#endif  // SRC_CORE_TRIANGLE_H_
