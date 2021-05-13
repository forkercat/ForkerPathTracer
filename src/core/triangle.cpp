//
// Created by Junhao Wang (@Forkercat) on 2021/5/12.
//

#include "triangle.h"

#include <spdlog/spdlog.h>

#include "bvh.h"

// Constructor
Triangle::Triangle(Point3f v0, Point3f v1, Point3f v2) : v0(v0), v1(v1), v2(v2)
{
    e1 = v1 - v0;
    e2 = v2 - v0;
    normal = Normalize(Cross(e1, e2));
}

bool Triangle::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    Float u{ 0.f }, v{ 0.f }, tNear{ -1 };

    if (rayIntersectMT(ray, tNear, u, v))
    {
        if (tNear > tMin && tNear < tMax)
        {
            hitRecord.t = tNear;
            hitRecord.p = ray.origin + tNear * ray.dir;
            // hitRecord.material
            // hitRecord.normal
            return true;
        }
    }

    return false;
}

Bounds3 Triangle::WorldBound() const
{
    return Union(Bounds3(v0, v1), v2);
}

Point3f Triangle::GetVertex(int index) const
{
    if (index == 0)
        return v0;
    else if (index == 1)
        return v1;
    else
        return v2;
}

// Möller–Trumbore: Get barycentric coordinates
bool Triangle::rayIntersectMT(const Ray& ray, float& tNear, float& u, float& v) const
{
    Vector3f s1 = Cross(ray.dir, e2);
    float    s1_dot_e1 = Dot(s1, e1);

    // Parallel
    if (abs(s1_dot_e1) < 0.001) return false;

    float inv = 1.f / s1_dot_e1;

    // u
    Vector3f s = ray.origin - v0;
    u = Dot(s1, s) * inv;
    if (u < 0 || u > 1) return false;

    // v
    Vector3f s2 = Cross(s, e1);
    v = Dot(s2, ray.dir) * inv;
    if (v < 0 || u + v > 1) return false;

    // t
    tNear = Dot(s2, e2) * inv;
    if (tNear < 0) return false;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

// Constructor
MeshTriangle::MeshTriangle(const std::string&               filename,
                           const std::shared_ptr<Material>& material)
    : m_Triangles(), m_Bvh(nullptr)
{
    // Load OBJ

    // -- set Triangle's material

    // Build BVH
    spdlog::info("Building MeshTriangle BVH...");
    m_Bvh = std::make_shared<BVHAccel>(*this);
}

bool MeshTriangle::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    if (m_Bvh)
    {
        return m_Bvh->Hit(ray, tMin, tMax, hitRecord);
    }
    else
    {
        HitRecord tempRecord;
        bool      hitAnything = false;

        Float closestSoFar = tMax;

        for (const auto& object : m_Triangles)
        {
            if (object->Hit(ray, tMin, closestSoFar, tempRecord))
            {
                hitAnything = true;
                closestSoFar = tempRecord.t;
                hitRecord = tempRecord;
            }
        }

        return hitAnything;
    }
}

Bounds3 MeshTriangle::WorldBound() const  // expensive
{
    Bounds3 worldBound(Point3f(0.f));

    for (const auto& object : m_Triangles)
    {
        worldBound = Union(worldBound, object->WorldBound());
    }

    return worldBound;
}