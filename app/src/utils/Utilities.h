#include <iostream>
#include "../obj/Object.h"

// Listbox Object Getter
bool sphereGetter(void* data, int index, const char** output)
{
    // When using Object* sphere names render incorrectly
    Sphere* objects = (Sphere*)data;
    Sphere& currentObject = objects[index];

    *output = currentObject.objectName.c_str();

    return true;
}