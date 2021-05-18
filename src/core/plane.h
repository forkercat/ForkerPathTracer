//
// Created by Junhao Wang (@Forkercat) on 2021/5/17.
//

#ifndef CORE_RECTANGLE_H_
#define CORE_RECTANGLE_H_

#include "hittable.h"
#include "material.h"
#include "triangle.h"

class Plane : public Hittable
{
public:
    Plane(Float width, Float height, const std::shared_ptr<Material>& mat)
        : material(mat), m_Triangles{ nullptr, nullptr }
    {
        Float widthOver2 = width / 2.f;
        Float heightOver2 = height / 2.f;

        // v3 -------- v2
        //  |          |
        // v0 -------- v1

        Point3f v0 = Point3f(-widthOver2, 0.f, heightOver2);
        Point3f v1 = Point3f(widthOver2, 0.f, heightOver2);
        Point3f v2 = Point3f(widthOver2, 0.f, -heightOver2);
        Point3f v3 = Point3f(-widthOver2, 0.f, -heightOver2);

        Vector2f t0 = Vector2f(0.f, 0.f);
        Vector2f t1 = Vector2f(1.f, 0.f);
        Vector2f t2 = Vector2f(1.f, 1.f);
        Vector2f t3 = Vector2f(0.f, 1.f);

        Vector3f n = Vector3f(0, 1, 0);

        m_Triangles[0] = std::make_shared<Triangle>(v0, v1, v2);
        m_Triangles[0]->SetNormals(n, n, n);
        m_Triangles[0]->SetTexCoords(t0, t1, t2);
        m_Triangles[0]->material = mat;

        m_Triangles[1] = std::make_shared<Triangle>(v0, v2, v3);
        m_Triangles[1]->SetNormals(n, n, n);
        m_Triangles[1]->SetTexCoords(t0, t2, t3);
        m_Triangles[1]->material = mat;
    }

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;

    void ApplyTransform(const Vector3f &translate, const Vector3f& rotate, Float scale) override
    {
        m_Triangles[0]->ApplyTransform(translate, rotate, scale);
        m_Triangles[1]->ApplyTransform(translate, rotate, scale);
    }

    Bounds3 WorldBound() const override
    {
        return Union(m_Triangles[0]->WorldBound(), m_Triangles[1]->WorldBound());
    }

    // Data
    std::shared_ptr<Material> material;

private:
    std::shared_ptr<Triangle> m_Triangles[2];
};

bool Plane::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    if (m_Triangles[0]->Hit(ray, tMin, tMax, hitRecord))
    {
        return true;
    }

    if (m_Triangles[1]->Hit(ray, tMin, tMax, hitRecord))
    {
        return true;
    }

    return false;

    //
    // // Hit
    // hitRecord.t = t;
    // hitRecord.p = ray(hitRecord.t);
    // hitRecord.SetFrontFace(ray, Vector3f(0, 0, 1));
    // hitRecord.material = material;
    // hitRecord.texCoord = Vector2f((x - xmin) / (xmax - xmin), (y - ymin) / (ymax -
    // ymin)); return true;
}

#endif  // CORE_RECTANGLE_H_
