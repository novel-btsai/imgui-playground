#pragma once

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "utils/utils.h"

void MainMenu(
    bool& show_main_menu,
    bool& show_lorise,
    const ImGuiIO& io)
{
    if (show_main_menu == false)
    {
        return;
    }

    // // This should go outside of the main loop in main.cpp
    // int my_image_width = 0;
    // int my_image_height = 0;
    // GLuint my_image_texture = 0;
    // bool ret = LoadTextureFromFile(
    //     "./rise.png", 
    //     &my_image_texture, 
    //     &my_image_width, 
    //     &my_image_height);
    // IM_ASSERT(ret);

    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin(
        "Main Menu",
        &show_main_menu,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);

    // ImGui::Image(
    //     (ImTextureID)(intptr_t)my_image_texture, 
    //     ImVec2(
    //         my_image_width, 
    //         my_image_height));

    TextAligned(
        "[LO-RISE]",
        0.5f,
        ImVec4(1,1,0,1));

    ImGui::NewLine();

    // What happens when launch button is pressed
    auto launch_callback = [&show_main_menu, &show_lorise] ()
    {
        show_main_menu = false;
        show_lorise = true;
    };

    ButtonAligned(
        "launch",
        ImVec2(50, 0),
        launch_callback,
        0.5f);

    ImGui::End();
}
