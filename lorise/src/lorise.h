#pragma once

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Lo-RISE includes
#include "controls.h"
#include "interactables.h"
#include "utils/utils.h"

/**
 * @brief Draw an agent at the designated location.
 */
void DrawAgent(
    const Agent& agent,
    const ImVec2 camera_pan,
    const float camera_zoom)
{
    // Apply camera pan offset 
    ImVec2 pos = ImVec2(
        agent.pos.x - camera_pan.x,
        agent.pos.y - camera_pan.y);

    // Name needs to be centered below the given position
    ImVec2 text_dims = ImGui::CalcTextSize(agent.name.c_str());
    ImGui::SetCursorPosX(pos.x - (text_dims.x / 2));
    ImGui::SetCursorPosY(pos.y - (text_dims.y / 2) + (Agent::ICON_SIZE * 2));
    ImGui::TextColored(
        (ImColor)agent.color,
        agent.name.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddNgon(
        pos,
        Agent::ICON_SIZE,
        agent.color,
        agent.air ?
            3 : 
            4);
}

/**
 * @brief Draw a tactic icon at the designated location.
 */
void DrawTacticIcon(
    const Tactic& tactic,
    const ImVec2 camera_pan,
    const float camera_zoom,
    bool dragging = false)
{
    // Apply camera pan offset 
    ImVec2 pos = ImVec2(
        tactic.pos.x - camera_pan.x,
        tactic.pos.y - camera_pan.y);

    // Account for user dragging a tactic
    if (dragging == true)
    {
        // TODO:
        // Bindings for each action need to be accessible anywhere.
        // Currently they are local vars in the respective action fnc.
        ImVec2 drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
        pos.x += drag.x;
        pos.y += drag.y;
    }

    // Name needs to be centered on given position
    ImVec2 text_dims = ImGui::CalcTextSize(tactic.name.c_str());
    ImGui::SetCursorPosX(pos.x - (text_dims.x / 2));
    ImGui::SetCursorPosY(pos.y - (text_dims.y / 2));
    ImGui::TextColored(
        (ImColor)tactic.color,
        tactic.name.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircle(
        pos,
        Tactic::ICON_SIZE,
        tactic.color);
}

/**
 * @brief Draws the grid background.
 */
void DrawGrid(
    const ImVec2 camera_pan,
    const float camera_zoom)
{
    const float cell_size = 100.0f;
    const ImU32 grid_color = IM_COL32(255, 255, 255, 20);
    ImVec2 window_dims = ImGui::GetWindowSize();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Example for columns indices:
    //
    // c-2 c-1 c0  c1  c2  c3  c4  c+1 c+2
    //           _______________
    //  |   |   |___|___|___|___|   |   |
    //  |   |   |___|___|___|___|   |   |
    //  |   |   |___|___|___|___|   |   |
    //  |   |   |___|___|___|___|   |   |
    //  
    // Top-left corner (top of c0) is origin.
    //
    // If cell size is 100 and the pan offset is -220,
    // then the first column on the left would be c-2 at -200
    // and the last column on the right would be c1 at 100.
    //
    // If cell size is 100 and the pan offset is 220,
    // then the first column on the left would be c3 at 300
    // and the last column on the right would be c+2 at 600.
    // 
    // Note:
    // When pan offset is negative, we include the (camera_pan / cell_size) index,
    // which is c-2 in the example.
    // When pan offset is positive, we exclude it, which is c2 in the example.

    // Add 1 to the end index to include all desired indices
    int ii_start = (camera_pan.x / cell_size) + (camera_pan.x > 0 ? 1 : 0);
    int ii_end = (window_dims.x / cell_size) + ii_start + 1;
    int jj_start = camera_pan.y / cell_size + (camera_pan.y > 0 ? 1 : 0);
    int jj_end = (window_dims.y / cell_size) + jj_start + 1;

    // Vertical lines
    for (int ii = ii_start;
        ii < ii_end;
        ii++)
    {
        float x = (ii * cell_size) - camera_pan.x;

        // Ignore columns that are out of view
        if (x < 0 ||
            x > window_dims.x)
        {
            continue;
        }

        // Columns ignore vertical pan offset because we 
        // always draw it from top to bottom of window
        ImVec2 top = ImVec2(
            x,
            0);

        ImVec2 bottom = ImVec2(
            x,
            window_dims.y);

        draw_list->AddLine(
            top,
            bottom,
            grid_color);
    }

    // Horizontal lines
    for (int jj = jj_start;
        jj < jj_end;
        jj++)
    {
        float y = (jj * cell_size) - camera_pan.y;

        // Ignore rows that are out of view
        if (y < 0 ||
            y > window_dims.y)
        {
            continue;
        }

        // Rows ignore horizontal pan offset because we 
        // always draw left to right of window
        ImVec2 left = ImVec2(
            0, 
            y);

        ImVec2 right = ImVec2(
            window_dims.x,
            y);

        draw_list->AddLine(
            left,
            right,
            grid_color);
    }
}

/**
 * @brief Draws the top down environmental view.
 */
void LoRISE(
    bool& show_lorise,
    const Action current_action,
    const ImVec2 camera_pan,
    const float camera_zoom,
    const Tactic* selected_tactic,
    const ImGuiIO& io,
    const std::vector<Agent>& agents,
    const std::vector<Tactic>& tactics)
{
    if (show_lorise == false)
    {
        return;
    }

    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin(
        "LO-RISE",
        &show_lorise,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);

    ImGui::Text("Zoom: %f", camera_zoom);
    DrawGrid(
        camera_pan,
        camera_zoom);

    for (Agent agent : agents)
    {
        DrawAgent(
            agent,
            camera_pan,
            camera_zoom);
    }

    for (Tactic tactic : tactics)
    {
        // TODO:
        // Will need a proper operator== for tactics instead of name matching
        bool dragging = 
            current_action == Action::DragTactic &&
            selected_tactic->name == tactic.name;

        DrawTacticIcon(
            tactic,
            camera_pan,
            camera_zoom,
            dragging);
    }

    ImGui::End();
}