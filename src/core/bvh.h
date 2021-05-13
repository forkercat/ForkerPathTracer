//
// Created by Junhao Wang (@Forkercat) on 2021/4/7.
//

#ifndef SRC_CORE_BVH_H_
#define SRC_CORE_BVH_H_

#include "geometry.h"
#include "hittable.h"

// Forward Declarations
class Scene;
class MeshTriangle;
struct BVHNode;

class BVHAccel : public Hittable
{
public:
    // Constructors
    explicit BVHAccel(const Scene& scene);
    explicit BVHAccel(const MeshTriangle& meshTriangle);
    explicit BVHAccel(const std::vector<std::shared_ptr<Hittable>>& objects);

    // Public Methods
    bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const override;
    Bounds3 WorldBound() const override;

    // Private Methods
    std::shared_ptr<BVHNode> recursiveBuild(
        std::vector<std::shared_ptr<Hittable>> objects);

private:
    std::shared_ptr<BVHNode>               m_Root;
    std::vector<std::shared_ptr<Hittable>> m_Objects;

    bool getHitRecord(std::shared_ptr<BVHNode> node, const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const;
};

// BVHNode
struct BVHNode
{
public:
    // Public Methods
    BVHNode() : bounds(), left(nullptr), right(nullptr), object(nullptr) { }

    // Public Data
    Bounds3                   bounds;
    std::shared_ptr<BVHNode>  left;
    std::shared_ptr<BVHNode>  right;
    std::shared_ptr<Hittable> object;
};

// class BVHNode : public Hittable
// {
// public:
//     // Constructors
//     BVHNode() = default;
//     BVHNode(const Scene& scene)
//         : BVHNode(scene.GetObjects(), 0, scene.GetObjects().size())
//     {
//     }
//
//     BVHNode(const std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end)
//     {
//     }
//
//     bool Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
//     override; Bounds3 WorldBound() const override;
//
//     // Public Data
//     shared_ptr<Hittable> left;
//     shared_ptr<Hittable> right;
//     Bounds3              bound;
// };
//
// bool BVHNode::Hit(const Ray& ray, Float tMin, Float tMax, HitRecord& hitRecord) const
// {
//     return false;
// }
//
// Bounds3 BVHNode::WorldBound() const
// {
//     return bound;
// }

#endif  // SRC_CORE_BVH_H_
