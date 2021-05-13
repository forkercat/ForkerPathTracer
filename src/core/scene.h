//
// Created by Junhao Wang (@Forkercat) on 2021/4/7.
//

#ifndef SRC_CORE_SCENE_H_
#define SRC_CORE_SCENE_H_

#include <vector>

#include "bounds.h"
#include "common.h"
#include "hittable.h"

class BVHAccel;

class Scene : public Hittable
{
public:
    // Constructors
    Scene() : m_Objects(), m_Bvh(nullptr) { }

    void Add(const std::shared_ptr<Hittable>& object) { m_Objects.push_back(object); }
    void Clear() { m_Objects.clear(); }

    const std::vector<std::shared_ptr<Hittable>>& GetObjects() const { return m_Objects; }

    void BuildBVH();

    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;

    inline bool SupportBVH() const { return m_Bvh != nullptr; }

private:
    // Private Data
    std::vector<std::shared_ptr<Hittable>> m_Objects;
    std::shared_ptr<BVHAccel>              m_Bvh;
};

#endif  // SRC_CORE_SCENE_H_
