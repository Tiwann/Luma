#pragma once
#include "Rendering/Texture.h"

namespace Luma::OpenGL
{
    class Device;

    class TextureImpl final : public Luma::Texture
    {
    public:
        TextureImpl() = default;
        ~TextureImpl() override = default;

        bool Initialize(const TextureCreateInfo& createInfo) override;
        void Destroy() override;
        bool IsValid() override;
        void Bind() const;

        uint32_t GetHandle() const;
    private:
        RenderDevice* m_Device = nullptr;
        uint32_t m_Handle = 0xFFFFFFFF;
        uint32_t m_TextureType = 0xFFFFFFFF;
    };
}
