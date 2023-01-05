
#include "Line.h"


Line::Line(const glm::vec3 &pOrigin,
           const glm::vec3 &pDestination,
           const glm::vec3 &pColor,
           float pThickness)
                : origin(pOrigin), destination(pDestination),
                  color(pColor), thickness(pThickness)
{
    int n;
    glm::vec3 error(0.0f, 0.0f, 0.0f);
    glm::vec3 coord = origin;
    glm::vec3 delta = destination - origin;

    glm::vec3 step(delta.x >= 0 ? 1 : -1,
                   delta.y >= 0 ? 1 : -1,
                   delta.z >= 0 ? 1 : -1);

    delta.x < 0 ? -delta.x : delta.x;
    delta.y < 0 ? -delta.y : delta.y;
    delta.z < 0 ? -delta.z : delta.z;

    // Number of Pixels / Length of the line to fill
    n = delta.x > delta.y ? delta.x : delta.y;
    n = n > delta.z ? n : delta.z;

    for (int i = 0; i < n; i++) {

        /* TODO Normally here we draw the line to pixels.
            We only want to draw pixels that are intersected though.
            Perhaps simply calling the intersect function here could work:
                -- Rays will probably not have access here --
            It's probable that the line doesn't need to be drawn.
         */

        error.x = delta.x;
        error.y = delta.y;
        error.z = delta.z;

        // Check if error exceeds the threshold
        if(((int)error.x << 1) >= n ){
            coord.x += step.x;
            error.x -= n;
        }
        if(((int)error.y << 1) >= n ){
            coord.y += step.y;
            error.y -= n;
        }
        if(((int)error.z << 1) >= n ){
            coord.z += step.z;
            error.z -= n;
        }
    }
    // Make sure the line has terminated at the correct place
    assert(coord.x == destination.x
        && coord.y == destination.y
        && coord.z == coord.z);
}

bool Line::intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &tNear) const
{
    /*  Solving for the intersection between two lines
     *    This is solved by finding points along either line.
     *
     *      p1 = r1 + t1*e1                             (1)
     *      p2 = r2 + t2*e2
     *
     *    t1 and t2 are scalar values along two lines respectively
     *    r1 and r2 are points along the two lines
     *    e1 and e2 are non-unit direction vectors of the lines
     *
     *    n = e1 x e2                                   (2)
     *      If the cross product is equal to 0 then the lines are parallel
     *
     *    d = ((n . p1) / ||n||) - ((n . p2) / ||n||)   (3)
     *      = (n . (p1 - p2)) / ||n||
     *      = (n . (r1 − r2+ t1*e1 − t2*e2)) / ||n||
     *
     *      Since the dot product of n.e1 and n.e2 = 0
     *      We can simplify further:
     *
     *    d = |(n . (r1 - r2))| / ||n||
     *
     *    To find the location of p1 and p2:
     *
     *    t1 = ((e2 x n) . (r2 - r1)) / n . n          (4)
     *    t2 = ((e1 x n) . (r2 - r1)) / n . n
     *
     *    Substitute t back into the first equation -> (1) to get
     *    the position on the line that is closest.
     *
     * */

    glm::vec3 lineDirection = GetLineDirection();
    glm::vec3 vec3null(0,0,0);

    glm::vec3 n = glm::cross(rayDirection, lineDirection);
    if (n == vec3null) {
        return false;   // The two lines are parallel
    }

    float dist = (abs(glm::dot(n, (rayOrigin - rayDirection)))) / glm::length(n);

    if (dist >= thickness) {
        float t1 = glm::dot(glm::cross(lineDirection, n), (origin - rayOrigin)) / glm::dot(n, n);
        float t2 = glm::dot(glm::cross(rayDirection, n), (origin - rayOrigin)) / glm::dot(n, n);

        glm::vec3 RayPoint  = rayOrigin + (t1 * rayDirection);
        glm::vec3 LinePoint = origin + (t2 * lineDirection);

        tNear = t1;

        if (dist == glm::length(RayPoint - LinePoint)) {
            return true;
        }
    }
}