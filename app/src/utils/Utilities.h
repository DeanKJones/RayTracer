#include <iostream>
#include "../obj/Object.h"

// Listbox Object Getter
bool sphereGetter(void* data, int index, const char** output)
{
    Object* objects = (Object*)data;
    Object& currentObject = objects[index];

    // not getting the right name
    std::string* name = &currentObject.objectName;
    std::string nameDeRef = *name;

    *output = name->data();

    return true;
}