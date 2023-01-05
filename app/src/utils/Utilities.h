#include <iostream>
#include "../obj/Object.h"

// Listbox Object Getter
bool objectGetter(void* data, int index, const char** output)
{
    Object** objects = (Object**)data;
//    Object currentObject = objects[index];
    *output = objects[index]->objectName.c_str();

    return true;
}