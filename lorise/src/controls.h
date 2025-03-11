#pragma once

// Standard library includes
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
    const ImVec2& camera_pan,
    std::vector<Tactic>& tactics)
{
    Tactic* ret = NULL;
    double best_dist_2 = -1;

    ImVec2 mouse_pos = ImGui::GetMousePos();

    // for (Tactic tactic : tactics)
    for (int ii = 0;
        ii < tactics.size();
        ii++)
    {
        Tactic tactic = tactics[ii];

        ImVec2 tactic_pos = ImVec2(
            tactic.pos.x - camera_pan.x,
            tactic.pos.y - camera_pan.y);
            
        double dist_2 = DistanceSquared(
            tactic_pos,
            mouse_pos);

        // Ignore if we aren't inside the icon
        if (dist_2 > Tactic::ICON_SIZE * Tactic::ICON_SIZE)
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
    Action& current_action,
    const ImVec2& camera_pan,
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
    selected_tactic = //selected_tactic ?: 
        GetTacticUnderMouse(
            camera_pan,
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

    // Apply drag to tactic position
    ImVec2 drag = ImGui::GetMouseDragDelta(binding);
    selected_tactic->pos.x += drag.x;
    selected_tactic->pos.y += drag.y;
}

/**
 * @brief Pan functionality that tracks the positional offset.
 * Screen coordinates use top-left as origin.
 */
ImVec2 PanCamera(
    Action& current_action,
    ImVec2& camera_pan)
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