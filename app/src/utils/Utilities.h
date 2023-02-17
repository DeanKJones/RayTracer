#include <iostream>
#include "../obj/Object.h"

// Listbox Object Getter
bool objectGetter(void* data, int index, const char** output)
{
    Object** objects = (Object**)data;
    *output = objects[index]->objectName.c_str();

    return true;
}

void keyBindsUI()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Key Bindings");

    ImGui::Text("Hold [ Spacebar ]: "
                     "\n    [ W ][ A ][ S ][ D ] movement"
                     "\n    [ Q ] travel down"
                     "\n    [ E ] travel up");
    ImGui::Separator();
    ImGui::Text("Hold [ Left Shift ]: "
                     "\n    Magnifying glass");
    ImGui::Separator();
    ImGui::Text("Key press [ R ]: "
                     "\n    Trace a single ray into the scene and render its path");
    ImGui::Separator();
    ImGui::Text("Key press [ T ]: "
                     "\n    Remove all ray paths from the scene");
    ImGui::Separator();

    ImGui::End();
    ImGui::PopStyleVar();
}