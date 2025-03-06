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

void Launch()
{
    // Close main menu window
    // Open LO-RISE window
}

void MainMenu(
    bool& show_main_menu,
    ImGuiIO& io)
{
    if (show_main_menu == false)
    {
        return;
    }

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
        "LORISE",
        &show_main_menu,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);

    // ImGui::Image(
    //     (ImTextureID)(intptr_t)my_image_texture, 
    //     ImVec2(
    //         my_image_width, 
    //         my_image_height));

    // Centered title
    // std::string title = "[LO-RISE]";
    // ImVec2 titleDims = ImGui::CalcTextSize(title.c_str());
    // ImGui::SetCursorPosX((windowDims.x - titleDims.x) * 0.5f);
    // ImGui::SetCursorPosY((windowDims.y - titleDims.y) * 0.5f);
    // ImGui::TextColored(
    //     ImVec4(1, 1, 0, 1),
    //     title.c_str());
    TextAligned(
        "[LO-RISE]",
        0.5f,
        ImVec4(1,1,0,1));

    ImGui::NewLine();

    ButtonAligned(
        "launch",
        ImVec2(50, 0),
        &Launch,
        0.5f);

    // ImVec2 windowDims = ImGui::GetWindowSize();
    // std::string btnText = "launch";
    // ImVec2 launchDims = ImGui::CalcTextSize(btnText.c_str());
    // ImGui::NewLine();
    // ImGui::SetCursorPosX((windowDims.x - launchDims.x) * 0.5f);
    // if (ImGui::Button(btnText.c_str()) == true)
    // {
    //     Launch();
    // }

    ImGui::End();
}
