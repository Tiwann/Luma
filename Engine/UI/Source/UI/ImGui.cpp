#include "Luma/UI/ImGui.h"

namespace ImGui
{
    void Image(Luma::IImguiRenderer* renderer, const Luma::ITextureView* textureView, const ImVec2& uv0, const ImVec2& uv1)
    {
        renderer->drawTexture(textureView, {uv0.x, uv0.y}, {uv1.x, uv1.y});
    }

    void Image(Luma::IImguiRenderer* renderer, const Luma::ITexture* texture, const ImVec2& uv0, const ImVec2& uv1)
    {
        renderer->drawTexture(texture->getTextureView(), {uv0.x, uv0.y}, {uv1.x, uv1.y});
    }
}
