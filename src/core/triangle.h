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

    void SetNormals(const Vector3f& n0_, const Vector3f& n1_, const Vector3f& n2_)
    {
        n0 = n0_;
        n1 = n1_;
        n2 = n2_;
    }

    void SetTexCoords(const Vector2f& t0_, const Vector2f& t1_, const Vector2f& t2_)
    {
        t0 = t0_;
        t1 = t1_;
        t2 = t2_;
    }

    void ApplyTransform(const Vector3f& translate, const Vector3f& rotate, Float scale) override
    {
        v0 = Transform(v0, translate, rotate, scale);
        v1 = Transform(v1, translate, rotate, scale);
        v2 = Transform(v2, translate, rotate, scale);

        e1 = v1 - v0;
        e2 = v2 - v0;

        // Vertex Normal
        n0 = Normalize(TransformNormal(n0, rotate));
        n1 = Normalize(TransformNormal(n1, rotate));
        n2 = Normalize(TransformNormal(n2, rotate));
    }

    // Public Data
    Point3f                   v0, v1, v2;
    Vector3f                  e1, e2;
    Vector2f                  t0, t1, t2;  // texture coords
    Vector3f                  n0, n1, n2;  // vertex normals
    std::shared_ptr<Material> material;

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

    int NumTriangles() const { return m_Triangles.size(); }

    std::shared_ptr<Triangle> GetTriangle(int index) const { return m_Triangles[index]; }

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

    void ApplyMaterial(const std::shared_ptr<Material>& material);

    void BuildBVH();

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    void ApplyTransform(const Vector3f &translate, const Vector3f& rotate, Float scale) override;

    Bounds3 WorldBound() const override;

private:
    std::string                            m_MeshName;
    std::vector<std::shared_ptr<Triangle>> m_Triangles;
    std::shared_ptr<BVHAccel>              m_Bvh;
};

#endif  // SRC_CORE_TRIANGLE_H_
