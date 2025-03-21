#pragma once

// Standard library includes
#include <algorithm>
#include <cmath>
#include <vector>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Lo-RISE includes
#include "interactables.h"
#include "utils/utils.h"

/**
 * @brief Possible interactions the user can perform.
 */
enum Action
{
    Idle,
    Pan,
    Zoom,
    DragTactic
};

/**
 * @brief Return a tactic if one is under the mouse.
 */
Tactic* GetTacticUnderMouse(
    const ImGuiIO& io,
    const ImVec2 camera_pan,
    const float camera_zoom,
    std::vector<Tactic>& tactics)
{
    Tactic* ret = NULL;
    double best_dist_2 = -1;

    ImVec2 mouse_pos = ImGui::GetMousePos();
    
    // NOTE:
    // Can't use ImGui::GetWindowSize() because 
    // Lo-RISE window hasn't been created yet
    ImVec2 window_dims = io.DisplaySize;
    ImVec2 center = ImVec2(
        window_dims.x / 2,
        window_dims.y / 2);

    for (int ii = 0;
        ii < tactics.size();
        ii++)
    {
        Tactic tactic = tactics[ii];

        ImVec2 tactic_pos = ImVec2(
            tactic.pos.x - camera_pan.x,
            tactic.pos.y - camera_pan.y);

        tactic_pos.x = center.x + (tactic_pos.x - center.x) * camera_zoom;
        tactic_pos.y = center.y + (tactic_pos.y - center.y) * camera_zoom;

        double dist_2 = Distance(
            tactic_pos,
            mouse_pos,
            true);

        float size = Tactic::ICON_SIZE * camera_zoom;

        // Ignore if we aren't inside the icon
        if (dist_2 > size * size)
        {
            continue;
        }

        // If icons are overlapping, pick the 
        // one that is closer to its center
        if (ret != NULL &&
            best_dist_2 < dist_2)
        {
            continue;
        }

        best_dist_2 = dist_2;
        ret = &tactics[ii];
    }

    return ret;
}

/**
 * @brief Check if mouse is trying to drag a tactic.
 */
void DragTacticIcon(
    const ImGuiIO& io,
    const ImVec2 camera_pan,
    const float camera_zoom,
    Action& current_action,
    std::vector<Tactic>& tactics,
    Tactic*& selected_tactic)
{
    // Tactic icon drag binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    // If we are doing a different action, exit
    if (current_action != Action::Idle &&
        current_action != Action::DragTactic)
    {
        return;
    }

    // Nothing to do if no tactic
    selected_tactic = selected_tactic ?: 
        GetTacticUnderMouse(
            io,
            camera_pan,
            camera_zoom,
            tactics);

    if (selected_tactic == NULL)
    {
        current_action = Action::Idle;
        return;
    }

    // Set action if tactic is present
    if (current_action == Action::Idle)
    {
        current_action = Action::DragTactic;
    }
    else if (current_action != Action::DragTactic)
    {
        return;
    }

    // Apply drag to tactic position on release
    if (ImGui::IsMouseReleased(binding) == true)
    {
        ImVec2 drag = ImGui::GetMouseDragDelta(binding);
        selected_tactic->pos.x += (drag.x / camera_zoom);
        selected_tactic->pos.y += (drag.y / camera_zoom);
    }
}

/**
 * @brief Pan functionality that tracks the positional offset.
 * Screen coordinates use top-left as origin.
 */
ImVec2 PanCamera(
    ImVec2& camera_pan,
    const float camera_zoom,
    Action& current_action)
{
    // Pan binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Left;

    // Set action to pan
    if (current_action == Action::Idle)
    {
        current_action = Action::Pan;
    }
    else if (current_action != Action::Pan)
    {
        return ImVec2(0, 0);
    }

    // If drag is 0, then we aren't panning
    // NOTE: Releasing mouse after dragging 
    // will have a final drag delta still 
    // (See ImGui::GetMouseDragDelta() description)
    ImVec2 drag = ImGui::GetMouseDragDelta(binding);
    drag.x /= camera_zoom;
    drag.y /= camera_zoom;

    if (drag.x == 0 &&
        drag.y == 0)
    {
        current_action = Action::Idle;
        return drag;
    }

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
 * @brief Zoom functionality that tracks the magnification factor.
 */
float ZoomCamera(
    float& camera_zoom,
    Action& current_action)
{
    // Zoom binding
    const ImGuiMouseButton binding = ImGuiMouseButton_Right;

    // Set action to zoom
    if (current_action == Action::Idle)
    {
        current_action = Action::Zoom;
    }
    else if (current_action != Action::Zoom)
    {
        return 1;
    }

    // Calculate zoom factor
    ImVec2 drag = ImGui::GetMouseDragDelta(binding);

    // Up = Zoom-in, Down = Zoom-out
    // Top-left corner is origin
    const float sensitivity = 100;
    float zoom = drag.y <= 0 ?
        1 - drag.y / sensitivity :
        1 - (drag.y / (2 * sensitivity));

    const float max_zoom = 2;
    const float min_zoom = 0.5;
    zoom = std::max(
        min_zoom / camera_zoom,
        std::min(
            zoom,
            max_zoom / camera_zoom));

    // Apply zoom factor
    if (ImGui::IsMouseReleased(binding) == true)
    {
        camera_zoom *= zoom;
        zoom = 1;
    }

    return zoom;
}