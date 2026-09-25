#include "Luma/Rendering/TextureUtils.h"
#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/Device.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include "Luma/Rendering/ImmediateExecutor.h"
#include "Luma/Memory/Ref.h"

#include <stb_image.h>


namespace Luma::TextureUtils
{
    using namespace RHI;

    bool uploadTextureDataSync(Device* device, Texture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize)
    {
        if (!device) return false;
        if (!texture) return false;
        if (!data) return false;
        if (!dataSize) return false;

        Ref<Buffer> stagingBuffer = BufferUtils::createStagingBuffer(device, data, dataSize);

        const ResourceState initialState = texture->getResourceState();

        Queue* renderQueue = device->getRenderQueue();

        TextureBarrier toCopyState;
        toCopyState.texture = texture;
        toCopyState.sourceAccess = getSourceAccessFlags(initialState);
        toCopyState.destAccess = getDestAccessFlags(ResourceState::CopyDest);
        toCopyState.destState = ResourceState::CopyDest;

        TextureBarrier toInitialState;
        toInitialState.texture = texture;
        toInitialState.sourceAccess = getSourceAccessFlags(ResourceState::CopyDest);
        toInitialState.destAccess = getDestAccessFlags(initialState);
        toInitialState.destState = initialState;

        ImmediateExecutor executor(device, renderQueue);
        executor.execute([&](CommandBuffer* cmdBuffer)
        {
            cmdBuffer->beginDebugGroup("Synchronous Texture Copy", Color::Orange);
            cmdBuffer->textureBarriers(toCopyState);
            cmdBuffer->copyBufferToTexture(stagingBuffer, 0, dataSize, texture, arrayIndex, mipLevel);
            cmdBuffer->textureBarriers(toInitialState);
            cmdBuffer->endDebugGroup();
        });

        return true;
    }

    bool generateMips(Device* device, Texture* texture, uint32_t sourceMip, uint32_t destMip, uint32_t mipCount)
    {

        Queue* renderQueue = device->getRenderQueue();
        Ref<CommandBuffer> cmdBuffer = device->createCommandBuffer(renderQueue);

        TextureBarrier barrier;
        barrier.sourceAccess = ResourceAccess::None;
        barrier.destAccess = ResourceAccess::CopyRead;
        barrier.destState = ResourceState::CopySource;

        TextureBarrier barrier2;
        barrier2.sourceAccess = ResourceAccess::None;

        return false;
    }

    Texture* loadTexture(Device* device, StringView filepath)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load(*filepath, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const TextureDesc textureDesc = TextureDesc::texture2D(width, height, Format::R8G8B8A8_SRGB, 1, 1);
        Texture* texture = device->createTexture(textureDesc);
        if (!texture) return nullptr;

        if (!uploadTextureDataSync(device, texture, 0, 0, pixels, pixelsSize))
        {
            texture->destroy();
            delete texture;
            stbi_image_free(pixels);
            return nullptr;
        }

        stbi_image_free(pixels);
        return texture;
    }

    Texture* loadTexture(Device* device, const void* data, uint64_t dataSize)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load_from_memory((const uint8_t*)data, (int)dataSize, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const TextureDesc createInfo = TextureDesc::texture2D(width, height, Format::R8G8B8A8_SRGB, 1, 1);
        Texture* texture = device->createTexture(createInfo);
        if (!texture) return nullptr;

        if (!uploadTextureDataSync(device, texture, 0, 0, pixels, pixelsSize))
        {
            texture->destroy();
            stbi_image_free(pixels);
            return nullptr;
        }

        stbi_image_free(pixels);
        return texture;
    }
}
