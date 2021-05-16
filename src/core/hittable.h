//
// Created by Junhao Wang (@Forkercat) on 2021/3/28.
//

#ifndef CORE_HITTABLE_H_
#define CORE_HITTABLE_H_

#include <memory>
#include <vector>

#include "bounds.h"
#include "constant.h"
#include "ray.h"

class Material;

// Hit Structs
struct HitRecord
{
    Point3f                   p;
    Vector3f                  normal;
    Float                     t;
    Vector2f                  texCoord;
    bool                      frontFace;
    std::shared_ptr<Material> material;

    HitRecord()
        : p(0.f), normal(0.f), t(Infinity), texCoord(), frontFace(false), material(nullptr) { }

    inline void SetFrontFace(const Ray& ray, const Vector3f& outwardNormal)
    {
        frontFace = Dot(ray.dir, outwardNormal) < 0.f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

// Hittable
class Hittable
{
public:
    virtual bool    Hit(const Ray& ray, Float tMin, Float tMax,
                        HitRecord& hitRecord) const = 0;
    virtual Bounds3 WorldBound() const = 0;
};

#endif  // CORE_HITTABLE_H_
