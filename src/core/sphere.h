//
// Created by Junhao Wang (@Forkercat) on 2021/3/28.
//

#ifndef CORE_SPHERE_H_
#define CORE_SPHERE_H_

#include <utility>

#include "hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(const Point3f& cen, Float rad, const std::shared_ptr<Material>& mat)
        : center(cen), radius(rad), material(mat){};

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;

    void ApplyTransform(const Vector3f &translate, Float rotateY, Float scale) override;

    Bounds3 WorldBound() const override
    {
        return Bounds3(center - Vector3f(radius), center + Vector3f(radius));
    }

    // Data
    Point3f              center;
    Float                radius;
    std::shared_ptr<Material> material;
};

void Sphere::ApplyTransform(const Vector3f& translate, Float rotateY, Float scale)
{
    center += translate;
    radius *= scale;
}

bool Sphere::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    Vector3f oc = ray.origin - center;  // A - C
    Float    a = Dot(ray.dir, ray.dir);
    Float    bOver2 = Dot(oc, ray.dir);
    Float    c = Dot(oc, oc) - radius * radius;

    Float discriminant = bOver2 * bOver2 - a * c;  // delta

    if (discriminant < 0) return false;

    Float sqrtDiscriminant = std::sqrt(discriminant);
    Float root = (-bOver2 - sqrtDiscriminant) / a;  // smaller root

    if (root < tMin || root > tMax)
    {
        root = (-bOver2 + sqrtDiscriminant) / a;  // larger root
        if (root < tMin || root > tMax) return false;
    }

    // Hit
    hitRecord.t = root;
    hitRecord.p = ray(hitRecord.t);
    Vector3f outwardNormal = (hitRecord.p - center) / radius;
    hitRecord.SetFrontFace(ray, outwardNormal);
    hitRecord.material = material;

    return true;
}

#endif  // CORE_SPHERE_H_
