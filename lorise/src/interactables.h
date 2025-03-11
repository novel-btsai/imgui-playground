#pragma once

// Standard library includes
#include <string>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/**
 * @brief Agent info. Would be replaced by the actual agent class.
 */
struct Agent
{
    static const ImU32 DEAD_COLOR = IM_COL32(255, 0, 0, 255);
    static const ImU32 ALIVE_COLOR = IM_COL32(0, 255, 0, 255);
    static const ImU32 TASKED_COLOR = IM_COL32(3, 252, 236, 255);
    static constexpr float ICON_SIZE = 10.0f;

    std::string name;
    bool air;
    bool dead;
    ImVec2 pos;
    ImU32 color;
};

/**
 * @brief Tactic info. Would be replaced by the actual tactic class.
 */
struct Tactic
{
    static const ImU32 COMPLETE_COLOR = IM_COL32(0, 255, 0, 255);
    static const ImU32 FAILED_COLOR = IM_COL32(255, 0, 0, 255);
    static const ImU32 WIP_COLOR = IM_COL32(255, 255, 255, 255);
    static constexpr float ICON_SIZE = 20.0f;

    std::string name;
    ImVec2 pos;
    ImU32 color;
};
