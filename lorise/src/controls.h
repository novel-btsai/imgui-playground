#pragma once

#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Forward declaration
struct Tactic;

/**
 * @brief Pan functionality that tracks the positional offset.
 * Screen coordinates use top-left as origin.
 */
ImVec2 Pan(ImVec2& camera_pan)
{
    // Pan binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    ImVec2 drag = ImGui::GetMouseDragDelta(binding);

    // Pan sums the mouse drag into the global 
    // positional offset if the mouse button is released
    if (ImGui::IsMouseReleased(binding) == true)
    {
        // Add the drag to offset to finalize
        camera_pan.x -= drag.x;
        camera_pan.y -= drag.y;
        drag = ImVec2(0, 0);
    }

    return drag;
}

/**
 * @brief Check if mouse is trying to drag a tactic.
 */
void DragTacticIcon(
    const ImVec2& camera_pan,
    const std::vector<Tactic>& tactics)
{
    // Tactic icon drag binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 drag = ImGui::GetMouseDragDelta(binding);

    if (ImGui::IsMouseDown(binding) == false)
    {
        return; 
    }

    // This function should be called during DrawTacticIcon????
    // see which icon we are on
    // apply drag to a selected icon
    // ???
    // profit
}
