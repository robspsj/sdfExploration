#pragma once
#include <imgui.h>

class AppUI
{
public:
    void Setup();
    void DrawWindow();
private:
    bool closed = true;
    ImTextureID textureId = 0;
};

