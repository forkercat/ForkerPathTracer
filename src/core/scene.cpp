//
// Created by Junhao Wang (@Forkercat) on 2021/4/7.
//

#include "scene.h"

#include <spdlog/spdlog.h>

#include "bvh.h"

void Scene::BuildBVH()
{
    spdlog::info("[Scene] Building BVH...");
    m_Bvh = std::make_shared<BVHAccel>(*this);
}

bool Scene::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
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

        for (const auto& object : m_Objects)
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

Bounds3 Scene::WorldBound() const  // expensive
{
    Bounds3 worldBound(Point3f(0.f));

    for (const auto& object : m_Objects)
    {
        worldBound = Union(worldBound, object->WorldBound());
    }

    return worldBound;
}