#include "Luma/Rendering/TextureUtils.h"
#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/GPUDevice.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include "Luma/Memory/Ref.h"

#include <stb_image.h>

namespace Luma::TextureUtils
{
    bool uploadTextureDataSync(IGPUDevice* device, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize)
    {
        if (!device) return false;
        if (!texture) return false;
        if (!data) return false;
        if (!dataSize) return false;

        Ref<IBuffer> stagingBuffer = BufferUtils::createStagingBuffer(device, data, dataSize);

        const EResourceState initialState = texture->getResourceState();

        IQueue* renderQueue = device->getRenderQueue();

        FTextureBarrier toCopyState;
        toCopyState.texture = texture;
        toCopyState.sourceAccess = getSourceAccessFlags(initialState);
        toCopyState.destAccess = getDestAccessFlags(EResourceState::CopyDest);
        toCopyState.destState = EResourceState::CopyDest;

        FTextureBarrier toInitialState;
        toInitialState.texture = texture;
        toInitialState.sourceAccess = getSourceAccessFlags(EResourceState::CopyDest);
        toInitialState.destAccess = getDestAccessFlags(initialState);
        toInitialState.destState = initialState;

        Ref<IFence> fence = device->createFence(0);

        Ref<ICommandBuffer> cmdBuffer = device->createCommandBuffer(renderQueue);

        cmdBuffer->begin();
        cmdBuffer->beginDebugGroup("Synchronous Texture Copy", FColor::Orange);
        cmdBuffer->textureBarriers(toCopyState);
        cmdBuffer->copyBufferToTexture(stagingBuffer, 0, dataSize, texture, arrayIndex, mipLevel);
        cmdBuffer->textureBarriers(toInitialState);
        cmdBuffer->endDebugGroup();
        cmdBuffer->end();

        FFenceSignal signal;
        signal.fence = fence;
        signal.value = 1;
        signal.stages = EPipelineStages::Copy;

        FQueueExecuteInfo copyExec;
        copyExec.cmdBuffers = {cmdBuffer};
        copyExec.signals = signal;
        renderQueue->executeCommandBuffers(copyExec);

        fence->waitOnCPU(1);

        return true;
    }

    ITexture* loadTexture(IGPUDevice* device, FStringView filepath)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load(*filepath, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const FTextureDesc textureDesc = FTextureDesc::texture2D(width, height, EFormat::R8G8B8A8_SRGB, 1, 1);
        ITexture* texture = device->createTexture(textureDesc);
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

    ITexture* loadTexture(IGPUDevice* device, const void* data, uint64_t dataSize)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load_from_memory((const uint8_t*)data, (int)dataSize, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const FTextureDesc createInfo = FTextureDesc::texture2D(width, height, EFormat::R8G8B8A8_SRGB, 1, 1);
        ITexture* texture = device->createTexture(createInfo);
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
