#include "Luma/Deko3D/SwapchainImpl.h"
#include "Luma/Deko3D/GpuDeviceImpl.h"
#include "Luma/Deko3D/Conversions.h"
#include "Luma/Deko3D/SwapchainPrivate.h"
#include "Luma/Memory/Memory.h"
#include <deko3d.h>


namespace Luma::Deko3d
{
    bool FSwapchainImpl::initialize(const FSwapchainDesc& swapchainDesc)
    {
        if (m_Private)
        {
            dkMemBlockDestroy(m_Private->imageMemBlock);
            delete m_Private;
        }
        m_Private = new Private;

        if (!swapchainDesc.device) return false;
        FGpuDeviceImpl* device = static_cast<FGpuDeviceImpl*>(swapchainDesc.device);
        const DkDevice deviceHandle = device->getHandle();

        DkImageLayoutMaker imageLayoutMaker;
        dkImageLayoutMakerDefaults(&imageLayoutMaker, deviceHandle);
        imageLayoutMaker.device = device->getHandle();
        imageLayoutMaker.type = DkImageType_2D;
        imageLayoutMaker.flags = DkImageFlags_UsageRender | DkImageFlags_UsagePresent | DkImageFlags_HwCompression;
        imageLayoutMaker.format = convert<DkImageFormat>(swapchainDesc.format);
        imageLayoutMaker.msMode = DkMsMode_1x;
        imageLayoutMaker.dimensions[0] = swapchainDesc.width;
        imageLayoutMaker.dimensions[1] = swapchainDesc.height;
        imageLayoutMaker.dimensions[2] = 1;
        imageLayoutMaker.mipLevels = 1;

        DkImageLayout imageLayout;
        dkImageLayoutInitialize(&imageLayout, &imageLayoutMaker);
        
        const uint64_t imageSize = dkImageLayoutGetSize(&imageLayout);
        const uint32_t imageAlignment = dkImageLayoutGetAlignment(&imageLayout);
        const uint32_t numImages = (uint32_t)swapchainDesc.buffering;
        const uint64_t totalBlockSize = numImages * Memory::alignUp(imageSize, imageAlignment);

        DkMemBlockMaker blockMaker;
        dkMemBlockMakerDefaults(&blockMaker, deviceHandle, totalBlockSize);
        blockMaker.flags = DkMemBlockFlags_GpuCached | DkMemBlockFlags_Image;

        m_Private->imageMemBlock = dkMemBlockCreate(&blockMaker);
        if (!m_Private->imageMemBlock) return false;

        DkImage* imagesPtrs[3]{nullptr, nullptr, nullptr};
        for (uint32_t i = 0; i < numImages; i++)
        {
            // Initialize each images in memory block
            const uint64_t offset = i * Memory::alignUp(imageSize, imageAlignment);
            dkImageInitialize(&m_Private->images[i], &imageLayout, m_Private->imageMemBlock, offset);
            imagesPtrs[i] = &m_Private->images[i];

            // Create default image view for it
            dkImageViewDefaults(&m_Private->imageViews[i], &m_Private->images[i]);
            m_Private->imageViews->type = DkImageType_2D;
            m_Private->imageViews->format = convert<DkImageFormat>(swapchainDesc.format);
            m_Private->imageViews->layerCount = 1;
            m_Private->imageViews->layerOffset = 0;
            m_Private->imageViews->mipLevelCount = 1;
            m_Private->imageViews->mipLevelOffset = 0;
        }


        DkSwapchainMaker maker;
        maker.device = device->getHandle();
        maker.numImages = (uint32_t)swapchainDesc.buffering;
        maker.pImages = imagesPtrs;
        maker.nativeWindow = device->getWindow();

        m_Handle = dkSwapchainCreate(&maker);
        if (!m_Handle) return false;

        m_Device = device;
        m_Buffering = swapchainDesc.buffering;
        m_ImageWidth = swapchainDesc.width;
        m_ImageHeight = swapchainDesc.height;
        m_ImageFormat = swapchainDesc.format;
        m_ImagePresentMode = swapchainDesc.presentMode;
        m_HasVSync = true; // I Guess ?
        m_Valid = true;
        return true;
    }

    void FSwapchainImpl::destroy()
    {
        dkSwapchainDestroy(m_Handle);
        if (m_Private)
        {
            dkMemBlockDestroy(m_Private->imageMemBlock);
            delete m_Private;
            m_Private = nullptr;
        }
    }

    bool FSwapchainImpl::acquireNextImage(IFence* fence, uint32_t& frameIndex)
    {
        //TODO:USE FENCE
        dkSwapchainAcquireImage(m_Handle, reinterpret_cast<int*>(&frameIndex), nullptr);
    }

    DkSwapchain FSwapchainImpl::getHandle() const
    {
        return m_Handle;
    }

    const DkSwapchain* FSwapchainImpl::getHandlePtr() const
    {
        return &m_Handle;
    }

    DkImage* FSwapchainImpl::getImage(uint32_t index) const
    {
        if (!m_Private) return nullptr;
        return &m_Private->images[index];
    }

    DkImageView* FSwapchainImpl::getImageView(uint32_t index) const
    {
        if (!m_Private) return nullptr;
        return &m_Private->imageViews[index];
    }


    bool FSwapchainImpl::isValid() const
    {
        return m_Valid;
    }

    ITexture* FSwapchainImpl::getTexture(uint32_t index)
    {

    }

    ITextureView* FSwapchainImpl::getTextureView(uint32_t index)
    {

    }

    void FSwapchainImpl::setName(FStringView name)
    {

    }

    void FSwapchainImpl::setSwapInterval(uint32_t swapInterval)
    {
        dkSwapchainSetSwapInterval(m_Handle, swapInterval);
    }
}
