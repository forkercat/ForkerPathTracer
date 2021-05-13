//
// Created by Junhao Wang (@Forkercat) on 2021/4/8.
//

#ifndef SRC_CORE_TRIANGLE_H_
#define SRC_CORE_TRIANGLE_H_

#include <memory>

#include "common.h"
#include "hittable.h"

class BVHAccel;

// Triangle Definitions
class Triangle : public Hittable
{
public:
    // Constructors
    Triangle(Point3f v0, Point3f v1, Point3f v2);

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;
    Point3f GetVertex(int index) const;

    // Public Data
    Point3f  v0, v1, v2;
    Vector3f e1, e2;
    Vector2f t0, t1, t2;  // texture coords
    Vector3f normal;

private:
    bool rayIntersectMT(const Ray& ray, float& tNear, float& u, float& v) const;
};

/////////////////////////////////////////////////////////////////////////////////

// MeshTriangle Definitions
class MeshTriangle : public Hittable
{
public:
    // Constructor
    MeshTriangle(const std::string& filename, const std::shared_ptr<Material>& material);

    const std::vector<std::shared_ptr<Hittable>>& GetTriangles() const
    {
        return m_Triangles;
    }

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_Triangles;
    std::shared_ptr<BVHAccel>              m_Bvh;
};

#endif  // SRC_CORE_TRIANGLE_H_
