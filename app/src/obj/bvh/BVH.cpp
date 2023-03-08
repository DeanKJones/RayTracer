#include "BVH.h"

#include <iostream>
#include "algorithm"

BVH_Node::BVH_Node(const Scene& scene)
{
    BVH_Node(scene.sceneObjects, 0, scene.sceneObjects.size());
}

BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Object>>& sceneObjects, size_t start, size_t end)
{
    m_BVHObjects = std::vector<std::shared_ptr<Object>>(sceneObjects);

    uint32_t axis = Core::Random::UInt(0, 2);
    auto comparator
                    = (axis == 0) ? boxCompare_x
                    : (axis == 1) ? boxCompare_y
                                  : boxCompare_z;

    size_t objectSpan = end - start;

    if (objectSpan == 1)
    {
        left = right = m_BVHObjects[start];
    }
    else if (objectSpan == 2)
    {
        if (comparator(m_BVHObjects[start], m_BVHObjects[start + 1]))
        {
            left  = m_BVHObjects[start];
            right = m_BVHObjects[start + 1];
        } else {
            left  = m_BVHObjects[start + 1];
            right = m_BVHObjects[start];
        }
    }
    else
    {
        std::sort(m_BVHObjects.begin() + start, m_BVHObjects.begin() + end, comparator);

        size_t mid = start + objectSpan / 2;
        left  = std::make_shared<BVH_Node>(m_BVHObjects, start, mid);
        right = std::make_shared<BVH_Node>(m_BVHObjects, mid, end);
    }

    AABB boxLeft;
    AABB boxRight;

    if (!left->intersectBB(boxLeft) || !right->intersectBB(boxRight)){
        std::cerr << "No bounding box in BVH_Node constructor." << "\n";
    }
    box = AABB::surroundingBox(boxLeft, boxRight);
}

bool BVH_Node::intersect(const Ray &ray, tHit &intersector, Payload &payload) const
{
    if(!box.intersect(ray, intersector))
    {
        return false;
    }

    bool hitLeft  = left->intersect(ray, intersector, payload);
    // Possible bug here
    if (hitLeft)
    {
        intersector.t_far = intersector.t_near;
    }
    bool hitRight = right->intersect(ray, intersector, payload);

    return hitLeft || hitRight;
}

bool BVH_Node::intersectBB(AABB &outputBox) const
{
    outputBox = box;
    return true;
}

bool BVH_Node::boxCompare(const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b, int axis)
{
    AABB boxA;
    AABB boxB;

    if (!a->intersectBB(boxA) || !b->intersectBB(boxB))
    {
        std::cerr << "No bounding box in BVH constructor." << "\n";
    }
    return boxA.min()[axis] < boxB.min()[axis];
}