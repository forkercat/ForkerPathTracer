//
// Created by Junhao Wang (@Forkercat) on 2021/5/17.
//

#ifndef CORE_RECTANGLE_H_
#define CORE_RECTANGLE_H_

#include "hittable.h"
#include "material.h"

class Rectangle : public Hittable
{
public:
    Rectangle() = default;
    Rectangle(Float x0, Float x1, Float y0, Float y1, Float z_,
              std::shared_ptr<Material> mat)
        : xmin(x0), xmax(x1), ymin(y0), ymax(y1), z(z_), material(mat)
    {
    }

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;

    Bounds3 WorldBound() const override
    {
        return Bounds3(Point3f(xmin, ymin, z - 0.0001f),
                       Point3f(xmax, ymax, z + 0.0001f));
    }

    // Data
    Float                     xmin, xmax, ymin, ymax, z;
    std::shared_ptr<Material> material;
};

bool Rectangle::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    Float t = (z - ray.origin.z) * ray.invDir.z;
    if (t < tMin || t > tMax)
    {
        return false;
    }

    Float x = ray.origin.x + t * ray.dir.x;
    Float y = ray.origin.y + t * ray.dir.y;

    if (x < xmin || x > xmax || y < ymin || y > ymax)
    {
        return false;
    }

    // Hit
    hitRecord.t = t;
    hitRecord.p = ray(hitRecord.t);
    hitRecord.SetFrontFace(ray, Vector3f(0, 0, 1));
    hitRecord.material = material;
    hitRecord.texCoord = Vector2f((x - xmin) / (xmax - xmin), (y - ymin) / (ymax - ymin));
    return true;
}

#endif  // CORE_RECTANGLE_H_
