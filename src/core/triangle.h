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
    Triangle(const Point3f& v0, const Point3f& v1, const Point3f& v2);

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;

    // Inlines
    Bounds3 WorldBound() const override { return Union(Bounds3(v0, v1), v2); }

    Point3f GetVertex(int index) const
    {
        if (index == 0)
            return v0;
        else if (index == 1)
            return v1;
        else
            return v2;
    }

    // Public Data
    Point3f  v0, v1, v2;
    Vector3f e1, e2;
    Vector2f t0, t1, t2;  // texture coords
    Vector3f n0, n1, n2;  // vertex normals
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
    explicit MeshTriangle(const std::string& meshName);

    int NumTriangles() const
    {
        return m_Triangles.size();
    }

    std::shared_ptr<Triangle> GetTriangle(int index) const
    {
        return m_Triangles[index];
    }

    std::vector<std::shared_ptr<Hittable>> GetTriangles() const
    {
        std::vector<std::shared_ptr<Hittable>> hittables;
        for (const auto& tri : m_Triangles)
        {
            hittables.push_back(tri);
        }
        return hittables;
    }

    std::string MeshName() const { return m_MeshName; }

    void AddTriangle(const std::shared_ptr<Triangle>& triangle)
    {
        m_Triangles.push_back(triangle);
    }

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;

private:
    std::string                            m_MeshName;
    std::vector<std::shared_ptr<Triangle>> m_Triangles;
    std::shared_ptr<BVHAccel>              m_Bvh;
};

#endif  // SRC_CORE_TRIANGLE_H_
