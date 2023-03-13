#include "BVH.h"

#include "algorithm"

BVH_Node::BVH_Node(const Scene& scene)
{
    BVH_Node(scene.sceneObjects, 0, scene.sceneObjects.size());
}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Object>>& sceneObjects, size_t start, size_t end)
{
    m_BVHObjects = sceneObjects;

    uint32_t axis = Core::Random::UInt(0, 2);

    auto comparator
                    = (axis == 0) ? boxCompare_x
                    : (axis == 1) ? boxCompare_y
                                  : boxCompare_z;

    size_t objectSpan = end - start;
#define leftNode 0
#if leftNode
    if (objectSpan == 1)
    {
        left = m_BVHObjects[start];
        right = m_Dummy;
        left->boundingBox(box);
        return;
    }
#else
    if (objectSpan == 3) {
        left  = std::make_shared<BVH_Node>(m_BVHObjects, start, start + 2);
        right = m_BVHObjects[start + 2];
    }
    else if (objectSpan == 2) {
        left  = m_BVHObjects[start];
        right = m_BVHObjects[start + 1];
    }
#endif
    else
    {
        std::sort(m_BVHObjects.begin() + start, m_BVHObjects.begin() + end, comparator);

        size_t mid = start + objectSpan / 2;
        left  = std::make_shared<BVH_Node>(m_BVHObjects, start, mid);
        right = std::make_shared<BVH_Node>(m_BVHObjects, mid, end);
    }

    AABB boxLeft;
    AABB boxRight;

    box = AABB(left->boundingBox(), right->boundingBox());
}

bool BVH_Node::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    if(!box.intersect(ray, intersector)) {
        return false;
    }

    bool hitLeft  = left->intersect(ray, intersector, payload);
    if (hitLeft)
    {
        intersector.t_far = payload.hitDistance;
    }
    bool hitRight = right->intersect(ray, intersector, payload);

    return hitLeft || hitRight;
}

AABB BVH_Node::boundingBox() const
{
    return box;
}

bool BVH_Node::boxCompare(const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b, int axis)
{
    return a->boundingBox().axis(axis).t_near < b->boundingBox().axis(axis).t_near;
}