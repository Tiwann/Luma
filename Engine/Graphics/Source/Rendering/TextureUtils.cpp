#include "Luma/Rendering/TextureUtils.h"
#include "Luma/Rendering/BufferUtils.h"
#include "Luma/Rendering/Buffer.h"
#include "Luma/Rendering/CommandBuffer.h"
#include "Luma/Rendering/Fence.h"
#include "Luma/Rendering/Queue.h"
#include "Luma/Rendering/GpuDevice.h"
#include "Luma/Rendering/ResourceBarrier.h"
#include "Luma/Memory/Ref.h"

#include <stb_image.h>

namespace Luma::TextureUtils
{
    bool uploadTextureData(IGpuDevice* device, ITexture* texture, uint32_t arrayIndex, uint32_t mipLevel,
                           const void* data, size_t dataSize)
    {
        if (!device) return false;
        if (!texture) return false;
        if (!data) return false;
        if (!dataSize) return false;

        Ref<IBuffer> stagingBuffer = BufferUtils::createStagingBuffer(device, data, dataSize);

        const EResourceState initialState = texture->getResourceState();

        IQueue* copyQueue = device->getCopyQueue();
        IQueue* renderQueue = device->getRenderQueue();

        FTextureBarrier toTransferBarrier;
        toTransferBarrier.texture = texture;
        toTransferBarrier.sourceAccess = getSourceAccessFlags(initialState);
        toTransferBarrier.destAccess = getDestAccessFlags(EResourceState::CopyDest);
        toTransferBarrier.destState = EResourceState::CopyDest;
        toTransferBarrier.sourceQueue = renderQueue;
        toTransferBarrier.destQueue = copyQueue;

        FTextureBarrier toInitialState;
        toInitialState.texture = texture;
        toInitialState.sourceAccess = getSourceAccessFlags(EResourceState::CopyDest);
        toInitialState.destAccess = getDestAccessFlags(initialState);
        toInitialState.destState = initialState;
        toInitialState.sourceQueue = copyQueue;
        toInitialState.destQueue = renderQueue;

        Ref<ICommandBuffer> cmdBuffer = device->createCommandBuffer(copyQueue);
        Ref<IFence> fence = device->createFence(0);

        cmdBuffer->begin();
        cmdBuffer->beginDebugGroup("Texture Copy", FColor::Orange);
        cmdBuffer->textureBarriers(toTransferBarrier);
        cmdBuffer->copyBufferToTexture(stagingBuffer, 0, dataSize, texture, arrayIndex, mipLevel);
        cmdBuffer->textureBarriers(toInitialState);
        cmdBuffer->endDebugGroup();
        cmdBuffer->end();

        FFenceSignal signal;
        signal.fence = fence;
        signal.value = 1;
        signal.stages = EPipelineStageBits::Copy;

        FQueueExecuteInfo execInfo;
        execInfo.cmdBuffers = {cmdBuffer};
        execInfo.signals = signal;

        copyQueue->executeCommandBuffers(execInfo);

        fence->waitOnCPU(1);
        return true;
    }

    ITexture* loadTexture(IGpuDevice* device, FStringView filepath)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load(*filepath, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const FTextureDesc createInfo = FTextureDesc::texture2D(width, height, EFormat::R8G8B8A8_SRGB, 1, 1);
        ITexture* texture = device->createTexture(createInfo);
        if (!texture) return nullptr;

        if (!uploadTextureData(device, texture, 0, 0, pixels, pixelsSize))
        {
            texture->destroy();
            stbi_image_free(pixels);
            return nullptr;
        }

        stbi_image_free(pixels);
        return texture;
    }

    ITexture* loadTexture(IGpuDevice* device, const void* data, uint64_t dataSize)
    {
        stbi_set_flip_vertically_on_load(true);
        int32_t width = 0, height = 0;
        stbi_uc* pixels = stbi_load_from_memory((const uint8_t*)data, (int)dataSize, &width, &height, nullptr, STBI_rgb_alpha);
        const size_t pixelsSize = width * height * 4 * sizeof(stbi_uc);
        if (!pixels) return nullptr;

        const FTextureDesc createInfo = FTextureDesc::texture2D(width, height, EFormat::R8G8B8A8_SRGB, 1, 1);
        ITexture* texture = device->createTexture(createInfo);
        if (!texture) return nullptr;

        if (!uploadTextureData(device, texture, 0, 0, pixels, pixelsSize))
        {
            texture->destroy();
            stbi_image_free(pixels);
            return nullptr;
        }

        stbi_image_free(pixels);
        return texture;
    }
}
