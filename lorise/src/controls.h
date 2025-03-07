#pragma once

#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Forward declaration
struct Tactic;

/**
 * @brief Calculates how much the mouse has dragged with a specific button.
 */
ImVec2 GetMouseDrag(
    const ImVec2& offset,
    ImVec2& mouse_start,
    ImGuiMouseButton btn)
{
    ImVec2 drag = ImVec2(0, 0);

    if (ImGui::IsMouseDown(btn) == false)
    {
        return drag;
    }

    ImVec2 mouse_pos = ImGui::GetMousePos();

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
    drag.x = mouse_start.x - mouse_pos.x;
    drag.y = mouse_start.y - mouse_pos.y;
    return drag;
}
/**
 * @brief Pan functionality that tracks the positional offset.
 * Screen coordinates use top-left as origin.
 */
ImVec2 Pan(
    ImVec2& offset,
    ImVec2& mouse_start)
{
    // Pan binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 drag = GetMouseDrag(
        offset,
        mouse_start,
        binding);

    // Pan sums the mouse drag into the global 
    // positional offset if the mouse button is released
    if (ImGui::IsMouseReleased(binding) == true &&
        mouse_start.x >= 0 &&
        mouse_start.y >= 0)
    {
        // Add the drag to offset to finalize
        offset.x += mouse_start.x - mouse_pos.x;
        offset.y += mouse_start.y - mouse_pos.y;
        mouse_start = ImVec2(-1, -1);
    }

    return drag;
}

/**
 * @brief Check if mouse is trying to drag a tactic.
 */
void DragTacticIcon(
    const ImVec2& offset,
    ImVec2& mouse_start,
    const std::vector<Tactic>& tactics)
{
    // Tactic icon drag binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 drag = GetMouseDrag(
        offset,
        mouse_start,
        binding);

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
