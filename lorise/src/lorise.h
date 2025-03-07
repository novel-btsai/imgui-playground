#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "controls.h"
#include "utils/utils.h"

/**
 * @brief Draw an agent at the designated location.
 */
void Agent(
    ImVec2 pos,
    std::string name,
    bool air,
    ImU32 color)
{
    // Size of the agent icon
    const float size = 10.0f;

    // Name needs to be centered below the given position
    ImVec2 text_dims = ImGui::CalcTextSize(name.c_str());
    ImGui::SetCursorPosX(pos.x - (text_dims.x / 2));
    ImGui::SetCursorPosY(pos.y - (text_dims.y / 2) + (size * 1.5f));
    ImGui::TextColored(
        (ImColor)color,
        name.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddNgon(
        pos,
        size,
        color,
        air ? 3 : 4);
}

/**
 * @brief Draw a tactic icon at the designated location.
 */
void TacticIcon(
    ImVec2 pos,
    std::string name,
    ImU32 color)
{
    // Size of the tactic icon
    const float size = 20.0f;

    // Name needs to be centered on given position
    ImVec2 text_dims = ImGui::CalcTextSize(name.c_str());
    ImGui::SetCursorPosX(pos.x - (text_dims.x / 2));
    ImGui::SetCursorPosY(pos.y - (text_dims.y / 2));
    ImGui::TextColored(
        (ImColor)color,
        name.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircle(
        pos,
        size,
        color);
}

/**
 * @brief Draws the grid background.
 */
void Grid(const ImVec2& offset)
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
    // If cell size is 100 and the offset is -220,
    // then the first column on the left would be c-2 at -200
    // and the last column on the right would be c1 at 100.
    //
    // If cell size is 100 and the offset is 220,
    // then the first column on the left would be c3 at 300
    // and the last column on the right would be c+2 at 600.
    // 
    // Note:
    // When offset is negative, we include the (offset / cell_size) index,
    // which is c-2 in the example.
    // When offset is positive, we exclude it, which is c2 in the example.

    // Add 1 to the end index to include all desired indices
    int ii_start = (offset.x / cell_size) + (offset.x > 0 ? 1 : 0);
    int ii_end = (window_dims.x / cell_size) + ii_start + 1;
    int jj_start = offset.y / cell_size + (offset.y > 0 ? 1 : 0);
    int jj_end = (window_dims.y / cell_size) + jj_start + 1;

    // Vertical lines
    for (int ii = ii_start;
        ii < ii_end;
        ii++)
    {
        float x = (ii * cell_size) - offset.x;

        // Ignore columns that are out of view
        if (x < 0 ||
            x > window_dims.x)
        {
            continue;
        }

        // Columns ignore vertical offset because we 
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
        float y = (jj * cell_size) - offset.y;

        // Ignore rows that are out of view
        if (y < 0 ||
            y > window_dims.y)
        {
            continue;
        }

        // Rows ignore horizontal offset because we 
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
    ImVec2& offset,
    const ImGuiIO& io)
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
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);

    ImGui::Text(
        "Offset: %f, %f",
        offset.x,
        offset.y);

    Grid(offset);

    ImVec2 mouse_pos = ImGui::GetMousePos();

    // Would want to loop through any active/idle tactics
    // and place their respective icons down, same with agents
    TacticIcon(
        mouse_pos,
        "ISR",
        IM_COL32(255, 0, 0, 255));

    mouse_pos.y -= 50.0f;
    Agent(
        mouse_pos,
        "BOB",
        false,
        IM_COL32(0, 255, 0, 255));

    ImGui::End();
}