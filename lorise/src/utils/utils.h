#pragma once

// Standard library includes
#include <functional> 

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Lo-RISE includes
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
 * @brief Visual debug tool to draw an X on a point.
 */
void DrawX(
    ImVec2 pos,
    ImU32 color = IM_COL32(255, 255, 255, 255))
{
    const float size = 5;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 top_left = ImVec2(
        pos.x - size,
        pos.y - size);

    ImVec2 top_right = ImVec2(
        pos.x + size,
        pos.y - size);

    ImVec2 bottom_left = ImVec2(
        pos.x - size,
        pos.y + size);

    ImVec2 bottom_right = ImVec2(
        pos.x + size,
        pos.y + size);

    draw_list->AddLine(
        top_left,
        bottom_right,
        color);

    draw_list->AddLine(
        top_right,
        bottom_left,
        color);
}

/**
 * @brief Get the distance between 2 ImVec2 points.
 */
double Distance(
    ImVec2 a,
    ImVec2 b,
    bool squared = false)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dist_2 = (dx * dx) + (dy * dy);
    return squared ? dist_2 : sqrt(dist_2);
}

/**
 * @brief Create colored text that is custom aligned.
 */
void TextAligned(
    const std::string text,
    const float align = 0.0f,
    const ImVec4 color = ImVec4(1, 1, 1, 1))
{
    ImVec2 window_dims = ImGui::GetWindowSize();
    ImVec2 text_dims = ImGui::CalcTextSize(text.c_str());
    ImGui::SetCursorPosX((window_dims.x - text_dims.x) * align);
    ImGui::SetCursorPosY((window_dims.y - text_dims.y) * align);
    ImGui::TextColored(
        color,
        text.c_str());
}

/**
 * @brief Create a button that is custom aligned.
 */
void ButtonAligned(
    const std::string text,
    const ImVec2 size,
    const std::function<void()>& callback,
    const float align = 0.0f)
{
    ImVec2 window_dims = ImGui::GetWindowSize();
    ImGui::SetCursorPosX((window_dims.x - size.x) * align);

    bool pressed = ImGui::Button(
        text.c_str(),
        size);

    if (pressed == true)
    {
        callback();
    }
}

/**
 * @brief Simple helper function to load an image into a OpenGL texture with common settings
 */ 
bool LoadTextureFromMemory(
    const void* data, 
    size_t data_size, 
    GLuint* out_texture, 
    int* out_width, 
    int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory(
        (const unsigned char*)data, 
        (int)data_size, 
        &image_width, 
        &image_height, 
        NULL, 
        4);

    if (image_data == NULL)
    {
        return false;
    }

    // Create a OpenGL texture identifier
    GLuint image_texture;

    glGenTextures(
        1, 
        &image_texture);

    glBindTexture(
        GL_TEXTURE_2D, 
        image_texture);

    // Setup filtering parameters for display
    glTexParameteri(
        GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, 
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, 
        GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(
        GL_UNPACK_ROW_LENGTH, 
        0);

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        image_width, 
        image_height, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        image_data);

    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

/**
 * @brief Open and read a file, then forward to LoadTextureFromMemory()
 */
bool LoadTextureFromFile(
    const char* file_name, 
    GLuint* out_texture, 
    int* out_width, 
    int* out_height)
{
    FILE* f = fopen(
        file_name, 
        "rb");

    if (f == NULL)
    {
        return false;
    }

    fseek(
        f, 
        0, 
        SEEK_END);

    size_t file_size = (size_t)ftell(f);

    if (file_size == -1)
    {
        return false;
    }

    fseek(
        f, 
        0, 
        SEEK_SET);

    void* file_data = IM_ALLOC(file_size);

    fread(
        file_data, 
        1, 
        file_size, 
        f);

    fclose(f);

    bool ret = LoadTextureFromMemory(
        file_data, 
        file_size, 
        out_texture, 
        out_width, 
        out_height);

    IM_FREE(file_data);
    return ret;
}