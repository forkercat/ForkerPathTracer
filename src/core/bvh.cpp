//
// Created by Junhao Wang (@Forkercat) on 2021/4/8.
//

#include "bvh.h"

#include <spdlog/spdlog.h>

#include <algorithm>

#include "scene.h"
#include "triangle.h"

BVHAccel::BVHAccel(const Scene& scene) : BVHAccel(scene.GetObjects())
{
}

BVHAccel::BVHAccel(const MeshTriangle& meshTriangle) : BVHAccel(meshTriangle.GetTriangles())
{
}

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Hittable>>& objects)
    : m_Objects(objects)
{
    time_t start, end;
    time(&start);

    if (objects.empty()) return;

    m_Root = recursiveBuild(objects);

    time(&end);
    Float diff = difftime(end, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = ((int)diff - (hrs * 3600) - (mins * 60));

    spdlog::info("BVH Generation Complete: {} hrs, {} mins, {} secs\n", hrs, mins, secs);
}

// Public Methods
bool BVHAccel::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
{
    if (m_Root != nullptr)
    {
        return getHitRecord(m_Root, ray, tMin, tMax, hitRecord);
    }
    return false;
}

Bounds3 BVHAccel::WorldBound() const
{
    return (m_Root != nullptr) ? m_Root->bounds : Bounds3();
}

// Private Methods

bool BVHAccel::getHitRecord(std::shared_ptr<BVHNode> node, const Ray& ray, Float tMin,
    Float tMax, HitRecord& hitRecord) const
{
    std::array<int, 3> dirIsNeg = { ray.dir.x < 0, ray.dir.y < 0, ray.dir.z < 0 };

    // Current Node
    if (!node->bounds.IntersectP(ray, ray.invDir, dirIsNeg, tMax))
    {
        return false;
    }

    // Check if current node is a leaf node
    if (node->object != nullptr)
    {
        return node->object->Hit(ray, tMin, tMax, hitRecord);
    }

    // Children Nodes
    HitRecord leftHitRecord;
    HitRecord rightHitRecord;
    bool hitLeft = getHitRecord(node->left, ray, tMin, tMax, leftHitRecord);
    bool hitRight = getHitRecord(node->right, ray, tMin, tMax, rightHitRecord);

    // Get the closest time
    if (hitLeft)
    {
        hitRecord = leftHitRecord;
    }

    if (hitRight && rightHitRecord.t < hitRecord.t)
    {
        hitRecord = rightHitRecord;
    }

    return hitLeft || hitRight;
}

std::shared_ptr<BVHNode> BVHAccel::recursiveBuild(
    std::vector<std::shared_ptr<Hittable>> objects)
{
    std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();

    if (objects.size() == 1)
    {
        // Create leaf
        node->bounds = objects[0]->WorldBound();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2)
    {
        // Create leaf at next depth
        node->left = recursiveBuild(std::vector<std::shared_ptr<Hittable>>{ objects[0] });
        node->right =
            recursiveBuild(std::vector<std::shared_ptr<Hittable>>{ objects[1] });
        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else  // >= 3
    {
        // Splitting into two nodes
        Bounds3 centroidBounds;
        for (const auto& object : objects)
        {
            centroidBounds = Union(centroidBounds, object->WorldBound().Centroid());
        }

        int dimension = centroidBounds.MaxExtent();
        switch (dimension)
        {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
            {
              return f1->WorldBound().Centroid().x < f2->WorldBound().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
            {
              return f1->WorldBound().Centroid().y < f2->WorldBound().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
            {
              return f1->WorldBound().Centroid().z < f2->WorldBound().Centroid().z;
            });
            break;
        }

        auto begin = objects.begin();
        auto middle = objects.begin() + (objects.size() / 2);
        auto end = objects.end();

        auto leftObjects = std::vector<std::shared_ptr<Hittable>>(begin, middle);
        auto rightObjects = std::vector<std::shared_ptr<Hittable>>(middle, end);

        node->left = recursiveBuild(leftObjects);
        node->right = recursiveBuild(rightObjects);

        node->bounds = Union(node->left->bounds, node->right->bounds);

        return node;
    }
}