#include <iostream>
#include "../obj/Object.h"

// Listbox Object Getter
bool objectGetter(void* data, int index, const char** output)
{
    Object** objects = (Object**)data;
    *output = objects[index]->objectName.c_str();

    return true;
}