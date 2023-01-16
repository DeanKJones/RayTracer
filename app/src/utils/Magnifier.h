#ifndef RAYTRACING_MAGNIFIER_H
#define RAYTRACING_MAGNIFIER_H

#include <iostream>

#include "src/Image.h"
#include "glm/glm.hpp"

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

// Magnifying glass
void MagnifingGlass(std::shared_ptr<Core::Image> image, ImVec2 cursorPos)
{
    const ImVec2 offset = ImGui::GetItemRectMin();

    float width  = -(float)image->GetWidth();
    auto height  =  (float)image->GetHeight();
    auto size = ImVec2(ImGui::GetContentRegionAvail().x, (ImGui::GetContentRegionAvail().x * height) / width);

    // Pixel position
    const ImVec2 center = (ImVec2(width, height) * (cursorPos - offset)) / size;

    const ImVec2 uv0 = (center + ImVec2(16, 16)) / ImVec2(width, height);
    const ImVec2 uv1 = (center - ImVec2(16, 16)) / ImVec2(width, height);

    // Create UI
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowPos(cursorPos - ImVec2(-25, 128));
    ImGui::BeginTooltip();

    // Render new image
    ImGui::Image(image->GetDescriptorSet(), ImVec2(128, 128), uv0, uv1);

    // Here we can define different pixel data to write to the ui
    ImGui::Text("Center: %f.1", fabs(center.x));
    ImGui::SameLine();
    ImGui::Text("Center: %f.1", fabs(center.y));

    ImGui::End();
    ImGui::PopStyleVar();
}

#endif //RAYTRACING_MAGNIFIER_H
