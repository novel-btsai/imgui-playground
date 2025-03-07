#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/**
 * @brief Pan functionality that tracks the positional offset.
 * Screen coordinates use top-left as origin.
 */
ImVec2 Pan(
    ImVec2& offset,
    ImVec2& mouse_start)
{
    ImVec2 drag = ImVec2(0, 0);
    ImVec2 mouse_pos = ImGui::GetMousePos();

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) == true)
    {
        // Store the initial click for dragging
        if (mouse_start.x < 0 &&
            mouse_start.y < 0)
        {
            if (mouse_pos.x >= 0 &&
                mouse_pos.y >= 0)
            {
                mouse_start = mouse_pos;
            }
        }
        // Track mouse drag movement
        else
        {
            drag.x = mouse_start.x - mouse_pos.x;
            drag.y = mouse_start.y - mouse_pos.y;
        }
    }
    else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) == true &&
        mouse_start.x >= 0 &&
        mouse_start.y >= 0)
    {
        // Add the drag to offset to finalize
        offset.x += mouse_start.x - mouse_pos.x;
        offset.y += mouse_start.y - mouse_pos.y;
        drag.x = 0;
        mouse_start = ImVec2(-1, -1);
    }

    return drag;
}