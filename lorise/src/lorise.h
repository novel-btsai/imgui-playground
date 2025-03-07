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

    // vertical lines
    for (int ii = 0;
        ii < window_dims.x / cell_size;
        ii++)
    {
        float x = ii * cell_size;

        ImVec2 top = ImVec2(
            x + offset.x, 
            0 + offset.y);

        ImVec2 bottom = ImVec2(
            x + offset.x, 
            window_dims.y + offset.y);

        draw_list->AddLine(
            top,
            bottom,
            grid_color);
    }

    // horizontal lines
    for (int jj = 0;
        jj < window_dims.y / cell_size;
        jj++)
    {
        float y = jj * cell_size;

        ImVec2 left = ImVec2(
            0 + offset.x, 
            y + offset.y);

        ImVec2 right = ImVec2(
            window_dims.x + offset.x,
            y + offset.y);

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