#include "AppUI.h"
#include <GL/gl3w.h>


char* CreateTexture(int width, int height)
{
    char* tex = new char[4 * width * height];

    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++)
        {
            size_t i = (y * width + x) * 4;

            tex[i + 0] = y * 255 / height;
            tex[i + 1] = x * 255 / width;
            tex[i + 2] = 0;
            tex[i + 3] = 255;
        }

    return tex;
}

ImTextureID GenerateTexture(char* tex, int width, int height)
{
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    return (ImTextureID)image_texture;
}

void AppUI::Setup()
{
    int width = 8;
    int height = 8;

    this->textureId = (ImTextureID)GenerateTexture(
        CreateTexture(width, height), width, height);
}

void AppUI::DrawWindow()
{
    ImGui::ShowDemoWindow();

    ImGui::Begin("Test");

    ImGui::Image(this->textureId, ImVec2(512, 512));

    ImGui::End();
}
