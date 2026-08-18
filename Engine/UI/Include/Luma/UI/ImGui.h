#pragma once
#include "Luma/Rendering/ImguiRenderer.h"
#include "Luma/Rendering/Texture.h"
#include "Luma/Rendering/TextureView.h"
#include <imgui.h>


namespace ImGui
{
    static void Image(Luma::IImguiRenderer* renderer, const Luma::ITextureView* textureView, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1));
    static void Image(Luma::IImguiRenderer* renderer, const Luma::ITexture* texture, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1));
}
