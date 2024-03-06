#pragma once
#include <imgui.h>

struct TextureHandler
{
    ImTextureID textureId;
    int width;
    int height;
};

class AppUI
{
public:
    void Setup();
    void DrawWindow();
private:
    bool closed = true;
    TextureHandler smallTextureId;
    TextureHandler bigTextureId;
};

