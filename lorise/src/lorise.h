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
    const ImVec2 camera_pan,
    const float camera_zoom,
    const Agent& agent)
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
    const ImVec2 camera_pan,
    const float camera_zoom,
    const Tactic& tactic,
    bool dragging = false)
{
    ImVec2 window_dims = ImGui::GetWindowSize();
    ImVec2 center = ImVec2(
        window_dims.x / 2,
        window_dims.y / 2);

    // Apply camera pan offset 
    ImVec2 pos = ImVec2(
        tactic.pos.x - camera_pan.x,
        tactic.pos.y - camera_pan.y);
    
    // Apply camera zoom
    pos.x = center.x + (pos.x - center.x) * camera_zoom;
    pos.y = center.y + (pos.y - center.y) * camera_zoom;

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
        Tactic::ICON_SIZE * camera_zoom,
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

    // Reference point for zoom
    ImVec2 center = ImVec2(
        window_dims.x / 2.0f,
        window_dims.y / 2.0f);

    // Find the screen offset from the zoom
    ImVec2 zoomed_window_dims = ImVec2(
        window_dims.x / camera_zoom,
        window_dims.y / camera_zoom);

    ImVec2 zoom_offsets = ImVec2(
        (zoomed_window_dims.x - window_dims.x) / 2,
        (zoomed_window_dims.y - window_dims.y) / 2);

    // Use camera pan and zoom offset to find 
    // which col/row begins at the top left corner.
    // Then calculate how many scaled cols/rows from there.
    // Add 2 to account for any integer division 
    // rounding and to include the last index.
    int col_start = (camera_pan.x - zoom_offsets.x) / cell_size;
    int col_end = col_start + (window_dims.x / (cell_size * camera_zoom)) + 2;
    int row_start = (camera_pan.y - zoom_offsets.y) / cell_size;
    int row_end = row_start + (window_dims.y / (cell_size * camera_zoom)) + 2;

    // Vertical lines
    for (int ii = col_start;
        ii < col_end;
        ii++)
    {
        // X-position with pan and zoom factor
        float x = (ii * cell_size) - camera_pan.x;
        x = center.x + (x - center.x) * camera_zoom;

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
    for (int jj = row_start;
        jj < row_end;
        jj++)
    {
        // Y-position with pan and zoom factor
        float y = (jj * cell_size) - camera_pan.y;
        y = center.y + (y - center.y) * camera_zoom;

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
    const ImGuiIO& io,
    const ImVec2 camera_pan,
    const float camera_zoom,
    const Action current_action,
    const Tactic* selected_tactic,
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

    DrawGrid(
        camera_pan,
        camera_zoom);

    for (Agent agent : agents)
    {
        DrawAgent(
            camera_pan,
            camera_zoom,
            agent);
    }

    for (Tactic tactic : tactics)
    {
        // TODO:
        // Will need a proper operator== for tactics instead of name matching
        bool dragging = 
            current_action == Action::DragTactic &&
            selected_tactic->name == tactic.name;

        DrawTacticIcon(
            camera_pan,
            camera_zoom,
            tactic,
            dragging);
    }

    ImGui::End();
}