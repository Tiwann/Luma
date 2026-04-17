#include "RenderDeviceImpl.h"
#include "CommandBufferImpl.h"
#include "FenceImpl.h"
#include "SamplerImpl.h"
#include "BufferImpl.h"
#include "TextureImpl.h"
#include "TextureViewImpl.h"

#include "Luma/Rendering/Buffer.h"
#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Containers/Array.h"

#include <iostream>
#include <Volk/volk.h>
#include <rgfw/rgfw.h>
#include <vma/vk_mem_alloc.h>

#ifndef VK_LAYER_KHRONOS_VALIDATION_NAME
#define VK_LAYER_KHRONOS_VALIDATION_NAME "VK_LAYER_KHRONOS_validation"
#endif

namespace Luma::Vulkan
{
    static VkBool32 messageCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                    const VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                    void* pUserData)
    {
        (void)messageTypes;
        (void)pUserData;

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            std::cout << "[VULKAN ERROR]: " << pCallbackData->pMessage << "\n";
            return true;
        }

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cout << "[VULKAN WARNING]: " << pCallbackData->pMessage << "\n";
            return true;
        }
        return false;
    };

    ERenderDeviceType FRenderDeviceImpl::getDeviceType()
    {
        return ERenderDeviceType::Vulkan;
    }

    bool FRenderDeviceImpl::initialize(const FRenderDeviceDesc& deviceDesc)
    {
        if (!deviceDesc.window)
        {
            std::wcerr << L"Failed to initialize render device: Invalid window!\n";
            return false;
        }

        if (deviceDesc.buffering == ESwapchainBuffering::None)
        {
            std::wcerr << L"Failed to initialize render device: Invalid buffering!\n";
            return false;
        }

        if (VK_FAILED(volkInitialize()))
        {
            std::wcerr << L"Failed to initialize Volk!\n";
            return false;
        }

        if (!s_Instance)
        {
            VkApplicationInfo applicationInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
            applicationInfo.apiVersion = VK_API_VERSION_1_4;
            applicationInfo.pEngineName = "Luma Engine";
            applicationInfo.engineVersion = 0;
            applicationInfo.pApplicationName = "Luma Engine";
            applicationInfo.applicationVersion = 0;

            Array<const char*> layers;
            layers.add(VK_LAYER_KHRONOS_VALIDATION_NAME);

            Array<const char*> extensions;
            extensions.add(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

            uint32_t rgfwExtensionCount = 0;
            const char** rgfwExtensions = RGFW_getRequiredInstanceExtensions_Vulkan(reinterpret_cast<size_t*>(&rgfwExtensionCount));
            extensions.addRange(rgfwExtensions, rgfwExtensionCount);

#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
            extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            VkInstanceCreateInfo instanceCreateInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
            instanceCreateInfo.pApplicationInfo = &applicationInfo;
            instanceCreateInfo.ppEnabledLayerNames = layers.data();
            instanceCreateInfo.enabledLayerCount = layers.count();
            instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
            instanceCreateInfo.enabledExtensionCount = extensions.count();

            if (VK_FAILED(vkCreateInstance(&instanceCreateInfo, nullptr, &s_Instance)))
            {
                std::wcerr << L"Failed to create vulkan instance!\n";
                return false;
            }

            volkLoadInstance(s_Instance);

#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
            if (!s_DebugMessenger)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
                debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
                debugMessengerCreateInfo.pfnUserCallback = messageCallback;
                if (vkCreateDebugUtilsMessengerEXT(s_Instance, &debugMessengerCreateInfo, nullptr, &s_DebugMessenger) != VK_SUCCESS)
                    return false;
            }
#endif
        }

        VkPhysicalDevice availablePhysicalDevices[32]{};
        uint32_t availablePhysicalDeviceCount = 0;
        vkEnumeratePhysicalDevices(s_Instance, &availablePhysicalDeviceCount, nullptr);
        vkEnumeratePhysicalDevices(s_Instance, &availablePhysicalDeviceCount, availablePhysicalDevices);

        if (availablePhysicalDeviceCount == 1)
        {
            m_PhysicalDevice = availablePhysicalDevices[0];
        }
        else
        {
            for (size_t physicalDeviceIndex = 0; physicalDeviceIndex < availablePhysicalDeviceCount; ++
                 physicalDeviceIndex)
            {
                const VkPhysicalDevice physicalDevice = availablePhysicalDevices[physicalDeviceIndex];
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(physicalDevice, &properties);

                VkPhysicalDeviceFeatures features;
                vkGetPhysicalDeviceFeatures(physicalDevice, &features);
                if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.samplerAnisotropy == (
                    VkBool32)true)
                {
                    m_PhysicalDevice = physicalDevice;
                    break;
                }
            }

            if (m_PhysicalDevice == nullptr)
                return false;
        }

        if (FDesktopWindow* window = dynamic_cast<FDesktopWindow*>(deviceDesc.window))
        {
            if (VK_FAILED(RGFW_window_createSurface_Vulkan(window->getHandle(), s_Instance, &m_Surface)))
            {
                std::wcerr << L"[VULKAN] Failed to create surface!\n";
                return false;
            }
        }

        uint32_t queueFamilyPropertiesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(m_PhysicalDevice, &queueFamilyPropertiesCount, nullptr);
        Array<VkQueueFamilyProperties2> queueFamilyProperties(queueFamilyPropertiesCount);
        for (VkQueueFamilyProperties2& properties : queueFamilyProperties)
            properties.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        vkGetPhysicalDeviceQueueFamilyProperties2(m_PhysicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

        for (uint32_t i = 0; i < queueFamilyProperties.count(); ++i)
        {
            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & (VK_QUEUE_GRAPHICS_BIT))
            {
                m_RenderQueue.setIndex(i);
                m_RenderQueue.setQueueType(EQueueType::Render);
            }

            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & (VK_QUEUE_COMPUTE_BIT))
            {
                m_ComputeQueue.setIndex(i);
                m_ComputeQueue.setQueueType(EQueueType::Compute);
            }

            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & (VK_QUEUE_TRANSFER_BIT))
            {
                m_CopyQueue.setIndex(i);
                m_CopyQueue.setQueueType(EQueueType::Copy);
            }
        }


        Array<VkDeviceQueueCreateInfo> queueCreateInfos;
        static constexpr float queuePriorities[] = { 1.0f };

        VkDeviceQueueCreateInfo renderQueueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        renderQueueCreateInfo.queueCount = 1;
        renderQueueCreateInfo.pQueuePriorities = queuePriorities;
        renderQueueCreateInfo.queueFamilyIndex = m_RenderQueue.getIndex();
        queueCreateInfos.add(renderQueueCreateInfo);

        VkDeviceQueueCreateInfo computeQueueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        computeQueueCreateInfo.queueCount = 1;
        computeQueueCreateInfo.pQueuePriorities = queuePriorities;
        computeQueueCreateInfo.queueFamilyIndex = m_ComputeQueue.getIndex();
        queueCreateInfos.add(computeQueueCreateInfo);

        VkDeviceQueueCreateInfo transferQueueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        transferQueueCreateInfo.queueCount = 1;
        transferQueueCreateInfo.pQueuePriorities = queuePriorities;
        transferQueueCreateInfo.queueFamilyIndex = m_CopyQueue.getIndex();
        queueCreateInfos.add(transferQueueCreateInfo);


        Array<const char*> deviceExtensions;
        deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        deviceExtensions.add(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);

        VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParametersFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES };
        shaderDrawParametersFeatures.shaderDrawParameters = true;

        VkPhysicalDeviceSynchronization2Features synchronization2Features = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES };
        synchronization2Features.synchronization2 = true;
        synchronization2Features.pNext = &shaderDrawParametersFeatures;

        VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES };
        indexingFeatures.pNext = &synchronization2Features;
        indexingFeatures.runtimeDescriptorArray = true;
        indexingFeatures.descriptorBindingVariableDescriptorCount = true;
        indexingFeatures.descriptorBindingPartiallyBound = true;
        indexingFeatures.shaderSampledImageArrayNonUniformIndexing = true;
        indexingFeatures.descriptorBindingSampledImageUpdateAfterBind = true;

        VkPhysicalDeviceIndexTypeUint8Features uint8Features = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES };
        uint8Features.indexTypeUint8 = true;
        uint8Features.pNext = &indexingFeatures;

        VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES };
        dynamicRenderingFeatures.dynamicRendering = true;
        dynamicRenderingFeatures.pNext = &uint8Features;

        VkPhysicalDeviceFeatures features = {};
        features.samplerAnisotropy = true;
        features.fillModeNonSolid = true;
        features.wideLines = true;

        VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        deviceCreateInfo.pNext = &dynamicRenderingFeatures;
        deviceCreateInfo.pEnabledFeatures = &features;
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.count();
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.count();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        if (VK_FAILED(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Handle)))
        {
            std::wcerr << L"Failed to create logical device!\n";
            return false;
        }

        volkLoadDevice(m_Handle);

        VkDeviceQueueInfo2 queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2 };
        queueInfo.queueIndex = 0;
        queueInfo.queueFamilyIndex = m_RenderQueue.getIndex();
        vkGetDeviceQueue2(m_Handle, &queueInfo, m_RenderQueue.getHandlePtr());
        if (!m_RenderQueue.getHandle())
        {
            std::wcerr << L"Failed to get render queue!\n";
            return false;
        }

        queueInfo.queueIndex = 0;
        queueInfo.queueFamilyIndex = m_ComputeQueue.getIndex();
        vkGetDeviceQueue2(m_Handle, &queueInfo, m_ComputeQueue.getHandlePtr());
        if (!m_ComputeQueue.getHandle())
        {
            std::wcerr << L"Failed to get compute queue!\n";
            return false;
        }

        queueInfo.queueIndex = 0;
        queueInfo.queueFamilyIndex = m_CopyQueue.getIndex();
        vkGetDeviceQueue2(m_Handle, &queueInfo, m_CopyQueue.getHandlePtr());
        if (!m_CopyQueue.getHandle())
        {
            std::wcerr << L"Failed to get copy queue!\n";
            return false;
        }

        if (!m_VkProcs) m_VkProcs = new VmaVulkanFunctions();

        VmaAllocatorCreateInfo allocatorCreateInfo = { 0 };
        allocatorCreateInfo.device = m_Handle;
        allocatorCreateInfo.instance = s_Instance;
        allocatorCreateInfo.physicalDevice = m_PhysicalDevice;
        allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;
        if (VK_FAILED(vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, m_VkProcs)))
        {
            std::wcerr << L"Failed to import vulkan functions pointers to vma!\n";
            return false;
        }

        allocatorCreateInfo.pVulkanFunctions = m_VkProcs;
        if (VK_FAILED(vmaCreateAllocator(&allocatorCreateInfo, &m_Allocator)))
        {
            std::wcerr << L"Failed to create Vulkan allocator!\n";
            return false;
        }

        //TODO: CREATE COMMANDS POOLS
        VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
        commandPoolCreateInfo.queueFamilyIndex = m_RenderQueue.getIndex();
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        if (VK_FAILED(vkCreateCommandPool(m_Handle, &commandPoolCreateInfo, nullptr, &m_RenderPool)))
        {
            std::wcerr << L"Failed to create render command pool!\n";
            return false;
        }

        commandPoolCreateInfo.queueFamilyIndex = m_ComputeQueue.getIndex();
        if (VK_FAILED(vkCreateCommandPool(m_Handle, &commandPoolCreateInfo, nullptr, &m_ComputePool)))
        {
            std::wcerr << L"Failed to create compute command pool!\n";
            return false;
        }

        commandPoolCreateInfo.queueFamilyIndex = m_CopyQueue.getIndex();
        if (VK_FAILED(vkCreateCommandPool(m_Handle, &commandPoolCreateInfo, nullptr, &m_CopyPool)))
        {
            std::wcerr << L"Failed to create copy command pool!\n";
            return false;
        }


        const auto GetPresentMode = [&](const bool vSync) -> EPresentMode
        {
            VkPresentModeKHR result;

            if (vSync)
            {
                result = VK_PRESENT_MODE_FIFO_KHR;

                uint32_t presentModeCount;
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, nullptr);
                Array<VkPresentModeKHR> presentModes(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, presentModes.data());

                if (presentModes.contains(VK_PRESENT_MODE_MAILBOX_KHR))
                    result = VK_PRESENT_MODE_MAILBOX_KHR;
            } else
            {
                result = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }

            switch (result)
            {
            case VK_PRESENT_MODE_IMMEDIATE_KHR: return EPresentMode::Immediate;
            case VK_PRESENT_MODE_MAILBOX_KHR: return EPresentMode::Mailbox;
            case VK_PRESENT_MODE_FIFO_KHR: return EPresentMode::Fifo;
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR: return EPresentMode::FifoRelaxed;
            default: return EPresentMode::Unknown;
            };
        };

        FSwapchainDesc swapchainDesc;
        swapchainDesc.device = this;
        swapchainDesc.buffering = deviceDesc.buffering;
        swapchainDesc.format = EFormat::R8G8B8A8_SRGB;
        swapchainDesc.width = deviceDesc.window->getWidth();
        swapchainDesc.height = deviceDesc.window->getHeight();
        swapchainDesc.presentMode = GetPresentMode(deviceDesc.vSync);
        if (!m_Swapchain.initialize(swapchainDesc))
        {
            std::wcerr << L"Failed to initialize swapchain!\n";
            return false;
        }

        for (size_t imageIndex = 0; imageIndex < m_Swapchain.getImageCount(); ++imageIndex)
        {
            VkSemaphoreCreateInfo semaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
            vkCreateSemaphore(m_Handle, &semaphoreCreateInfo, nullptr, &m_Frames[imageIndex].presentSemaphore);
            vkCreateSemaphore(m_Handle, &semaphoreCreateInfo, nullptr, &m_Frames[imageIndex].submitSemaphore);

            m_Frames[imageIndex].fence.initialize(FFenceDesc(this));
            m_Frames[imageIndex].cmdBuffer.initialize(FCommandBufferDesc(this, EQueueType::Render));
        }

        //TODO: DESCRIPTOR POOLS


        m_ImmediateExecutor.initialize({this, &m_RenderQueue});
        m_Window = deviceDesc.window;
        s_DeviceCount++;
        return true;
    }

    void FRenderDeviceImpl::destroy()
    {
        waitIdle();
        m_ImmediateExecutor.destroy();
        for (size_t imageIndex = 0; imageIndex < m_Swapchain.getImageCount(); ++imageIndex)
        {
            vkDestroySemaphore(m_Handle, m_Frames[imageIndex].submitSemaphore, nullptr);
            vkDestroySemaphore(m_Handle, m_Frames[imageIndex].presentSemaphore, nullptr);
            m_Frames[imageIndex].fence.destroy();
            m_Frames[imageIndex].cmdBuffer.destroy();
            m_Frames[imageIndex].submitSemaphore = nullptr;
            m_Frames[imageIndex].presentSemaphore = nullptr;
        }

        vkDestroyCommandPool(m_Handle, m_CopyPool, nullptr);
        vkDestroyCommandPool(m_Handle, m_ComputePool, nullptr);
        vkDestroyCommandPool(m_Handle, m_RenderPool, nullptr);
        //TODO: DESTROY COMMAND POOLS

        m_Swapchain.destroy();
        vmaDestroyAllocator(m_Allocator);
        vkDestroySurfaceKHR(s_Instance, m_Surface, nullptr);
        vkDestroyDevice(m_Handle, nullptr);

        s_DeviceCount--;
        if (s_DeviceCount <= 0)
        {
#if defined(LUMA_DEBUG) || defined(LUMA_DEV)
            vkDestroyDebugUtilsMessengerEXT(s_Instance, s_DebugMessenger, nullptr);
#endif
            vkDestroyInstance(s_Instance, nullptr);
        }
    }

    bool FRenderDeviceImpl::beginFrame()
    {
        const FDesktopWindow* window = dynamic_cast<FDesktopWindow*>(m_Window);
        if (window->isMinimized())
            return false;

        if (!m_Swapchain.isValid())
        {
            vkDeviceWaitIdle(m_Handle);
            m_Swapchain.resize(window->getWidth(), window->getHeight());
            m_CurrentFrameIndex = 0;
            return false;
        }

        FFenceImpl& fence = m_Frames[m_LastFrameIndex].fence;
        fence.wait(FENCE_WAIT_INFINITE);
        fence.reset();

        const VkSemaphore& presentSemaphore = m_Frames[m_LastFrameIndex].presentSemaphore;
        if (!m_Swapchain.acquireNextImage(presentSemaphore, nullptr, m_CurrentFrameIndex))
        {
            m_Swapchain.invalidate();
            return false;
        }

        FCommandBufferImpl& commandBuffer = m_Frames[m_CurrentFrameIndex].cmdBuffer;
        if (!commandBuffer.begin())
            return false;

        VkImageMemoryBarrier2 barrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcAccessMask = VK_ACCESS_2_NONE;
        barrier.dstAccessMask = VK_ACCESS_2_NONE;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_Swapchain.getImage(m_CurrentFrameIndex);
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcStageMask = 0;
        barrier.dstStageMask = 0;

        VkDependencyInfo dependency = { VK_STRUCTURE_TYPE_DEPENDENCY_INFO };
        dependency.dependencyFlags = 0;
        dependency.imageMemoryBarrierCount = 1;
        dependency.pImageMemoryBarriers = &barrier;
        vkCmdPipelineBarrier2(commandBuffer.getHandle(), &dependency);
        return true;
    }

    void FRenderDeviceImpl::endFrame()
    {
        FCommandBufferImpl& cmdBuffer = m_Frames[m_CurrentFrameIndex].cmdBuffer;

        VkImageMemoryBarrier2 barrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcAccessMask = VK_ACCESS_2_NONE;
        barrier.dstAccessMask = VK_ACCESS_2_NONE;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_Swapchain.getImage(m_CurrentFrameIndex);
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcStageMask = 0;
        barrier.dstStageMask = 0;

        VkDependencyInfo inDependency = { VK_STRUCTURE_TYPE_DEPENDENCY_INFO };
        inDependency.dependencyFlags = 0;
        inDependency.imageMemoryBarrierCount = 1;
        inDependency.pImageMemoryBarriers = &barrier;
        vkCmdPipelineBarrier2(cmdBuffer.getHandle(), &inDependency);

        FFenceImpl& fence = m_Frames[m_LastFrameIndex].fence;
        const VkSemaphore submitSemaphore = m_Frames[m_CurrentFrameIndex].submitSemaphore;
        const VkSemaphore presentSemaphore = m_Frames[m_LastFrameIndex].presentSemaphore;
        cmdBuffer.end();

        m_RenderQueue.submit(&cmdBuffer, presentSemaphore, submitSemaphore, &fence, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    }

    void FRenderDeviceImpl::present()
    {
        const VkSemaphore submitSemaphore = m_Frames[m_CurrentFrameIndex].submitSemaphore;
        if (!m_RenderQueue.present(&m_Swapchain, submitSemaphore, m_CurrentFrameIndex))
            m_Swapchain.invalidate();
        m_LastFrameIndex = m_CurrentFrameIndex;
    }

    void FRenderDeviceImpl::waitIdle()
    {
        vkDeviceWaitIdle(m_Handle);
    }

    uint32_t FRenderDeviceImpl::getFrameCount()
    {
        return m_Swapchain.getImageCount();
    }

    uint32_t FRenderDeviceImpl::getCurrentFrameIndex()
    {
        return m_CurrentFrameIndex;
    }

    bool FRenderDeviceImpl::hasVSync()
    {
        return m_Swapchain.hasVSync();
    }

    ISwapchain* FRenderDeviceImpl::getSwapchain()
    {
        return &m_Swapchain;
    }

    IQueue* FRenderDeviceImpl::getRenderQueue()
    {
        return &m_RenderQueue;
    }

    IQueue* FRenderDeviceImpl::getComputeQueue()
    {
        return &m_ComputeQueue;
    }

    IQueue* FRenderDeviceImpl::getCopyQueue()
    {
        return &m_CopyQueue;
    }

    IBuffer* FRenderDeviceImpl::createBuffer(const FBufferDesc& bufferDesc) const
    {
        FBufferDesc desc(bufferDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FBufferImpl* buffer = new FBufferImpl();
        if (!buffer->initialize(desc))
        {
            buffer->destroy();
            delete buffer;
            return nullptr;
        }
        return buffer;
    }

    ITexture* FRenderDeviceImpl::createTexture(const FTextureDesc& textureDesc) const
    {
        FTextureDesc desc(textureDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FTextureImpl* texture = new FTextureImpl();
        if (!texture->initialize(desc))
        {
            texture->destroy();
            delete texture;
            return nullptr;
        }
        return texture;
    }

    ITextureView* FRenderDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc) const
    {
        FTextureViewDesc desc(textureViewDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FTextureViewImpl* textureView = new FTextureViewImpl();
        if (!textureView->initialize(desc))
        {
            textureView->destroy();
            delete textureView;
            return nullptr;
        }
        return textureView;
    }

    IShader* FRenderDeviceImpl::createShader(const FShaderDesc& shaderDesc) const
    {
        return nullptr;
    }

    ICommandBuffer* FRenderDeviceImpl::createCommandBuffer(const FCommandBufferDesc& commandBufferDesc) const
    {
        FCommandBufferDesc desc(commandBufferDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FCommandBufferImpl* cmdBuffer = new FCommandBufferImpl();
        if (!cmdBuffer->initialize(desc))
        {
            cmdBuffer->destroy();
            delete cmdBuffer;
            return nullptr;
        }
        return cmdBuffer;
    }

    ISampler* FRenderDeviceImpl::createSampler(const FSamplerDesc& samplerDesc) const
    {
        FSamplerDesc desc(samplerDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FSamplerImpl* sampler = new FSamplerImpl();
        if (!sampler->initialize(desc))
        {
            sampler->destroy();
            delete sampler;
            return nullptr;
        }
        return sampler;
    }

    ISampler* FRenderDeviceImpl::getOrCreateSampler(const FSamplerDesc& samplerDesc)
    {
        return nullptr;
    }


    IGraphicsPipeline* FRenderDeviceImpl::createGraphicsPipeline(const FGraphicsPipelineDesc& pipelineDesc) const
    {
        return nullptr;
    }

    IComputePipeline* FRenderDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc) const
    {
        return nullptr;
    }

    IFence* FRenderDeviceImpl::createFence(const FFenceDesc& fenceDesc) const
    {
        FFenceDesc desc(fenceDesc);
        desc.device = const_cast<FRenderDeviceImpl*>(this);
        FFenceImpl* fence = new FFenceImpl();
        if (!fence->initialize(desc))
        {
            fence->destroy();
            delete fence;
            return nullptr;
        }
        return fence;
    }

    VkCommandPool FRenderDeviceImpl::getCommandPool(const EQueueType queueType) const
    {
        switch (queueType)
        {
        case EQueueType::None: return nullptr;
        case EQueueType::Render: return m_RenderPool;
        case EQueueType::Compute: return m_ComputePool;
        case EQueueType::Copy: return m_CopyPool;
        default: return m_RenderPool;
        }
    }

    FImmediateExecutorImpl& FRenderDeviceImpl::getExecutor()
    {
        return m_ImmediateExecutor;
    }
}
