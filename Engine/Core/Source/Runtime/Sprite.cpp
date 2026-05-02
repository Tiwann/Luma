#include "Luma/Runtime/Sprite.h"
#include "Luma/Rendering/Texture.h"

namespace Luma
{
    Sprite Sprite::fromTexture(WeakRef<ITexture> texture)
    {
        if (!texture) return {0, 0, 0, 0, nullptr};
        Sprite sprite;
        sprite.texture = texture;
        sprite.x = 0;
        sprite.y = 0;
        sprite.width = texture->getWidth();
        sprite.height = texture->getHeight();
        return sprite;
    }
}
