#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils/utils.h"

void LoRISE(
    bool& show_lorise,
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

    TextAligned(
        "PLEASE PRESENT YOUR ID CITIZEN",
        0.5f,
        ImVec4(1,1,1,1));

    ImGui::End();
}