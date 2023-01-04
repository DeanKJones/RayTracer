
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