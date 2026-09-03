#include "Luma/Vulkan/GpuDeviceImpl.h"
#include "Luma/Vulkan/CommandBufferImpl.h"
#include "Luma/Vulkan/FenceImpl.h"
#include "Luma/Vulkan/SamplerImpl.h"
#include "Luma/Vulkan/BufferImpl.h"
#include "Luma/Vulkan/TextureImpl.h"
#include "Luma/Vulkan/TextureViewImpl.h"
#include "Luma/Vulkan/ShaderImpl.h"
#include "Luma/Vulkan/ComputePipelineImpl.h"
#include "Luma/Vulkan/RenderPipelineImpl.h"
#include "Luma/Runtime/DesktopWindow.h"
#include "Luma/Containers/Array.h"
#include "Luma/Containers/StringFormat.h"
#include "Luma/Vulkan/Conversions.h"
#include "Luma/Vulkan/VulkanUtils.h"

#include <iostream>

#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>


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
            std::cout << "[VULKAN ERROR]: " << pCallbackData->pMessage << std::endl;
            return false;
        }

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cout << "[VULKAN WARNING]: " << pCallbackData->pMessage << std::endl;
            return false;
        }

        return false;
    };

    EGpuDeviceType FGpuDeviceImpl::getDeviceType()
    {
        return EGpuDeviceType::Vulkan;
    }

    bool FGpuDeviceImpl::initialize(const FGpuDeviceDesc& deviceDesc)
    {
        if (!deviceDesc.window)
        {
            std::wcerr << L"Failed to initialize render device: Invalid window!" << std::endl;
            return false;
        }

        if (deviceDesc.buffering == ESwapchainBuffering::None)
        {
            std::wcerr << L"Failed to initialize render device: Invalid buffering!" << std::endl;
            return false;
        }

        if (VK_FAILED(volkInitialize()))
        {
            std::wcerr << L"Failed to initialize Volk!" << std::endl;
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

            TArray<const char*> layers;
#ifdef LUMA_DEBUG
            layers.add(VK_LAYER_KHRONOS_VALIDATION_NAME);
#endif

            TArray<const char*> extensions;
            extensions.add(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            extensions.addRange(glfwExtensions, glfwExtensionCount);
            extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

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

#if defined(LUMA_DEBUG)
            if (!s_DebugMessenger)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
                debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
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

        VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptorBufferProperties { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT };
        VkPhysicalDeviceProperties2 physicalDeviceProperties { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, &descriptorBufferProperties };
        vkGetPhysicalDeviceProperties2(m_PhysicalDevice, &physicalDeviceProperties);

        m_DescriptorBufferProperties.descriptorBufferOffsetAlignment = descriptorBufferProperties.descriptorBufferOffsetAlignment;
        m_DescriptorBufferProperties.uniformBufferDescriptorSize = descriptorBufferProperties.uniformBufferDescriptorSize;
        m_DescriptorBufferProperties.maxDescriptorBufferBindings = descriptorBufferProperties.maxDescriptorBufferBindings;
        m_DescriptorBufferProperties.uniformBufferDescriptorSize = descriptorBufferProperties.uniformBufferDescriptorSize;
        m_DescriptorBufferProperties.storageBufferDescriptorSize = descriptorBufferProperties.storageBufferDescriptorSize;
        m_DescriptorBufferProperties.sampledImageDescriptorSize = descriptorBufferProperties.sampledImageDescriptorSize;
        m_DescriptorBufferProperties.storageImageDescriptorSize = descriptorBufferProperties.storageImageDescriptorSize;
        m_DescriptorBufferProperties.samplerDescriptorSize = descriptorBufferProperties.samplerDescriptorSize;
        m_DescriptorBufferProperties.combinedImageSamplerDescriptorSize = descriptorBufferProperties.combinedImageSamplerDescriptorSize;

        if (FDesktopWindow* window = dynamic_cast<FDesktopWindow*>(deviceDesc.window))
        {
            if (VK_FAILED(glfwCreateWindowSurface(s_Instance, window->getHandle(), nullptr, &m_Surface)))
            {
                std::wcerr << L"[VULKAN] Failed to create surface!\n";
                return false;
            }
        }

        uint32_t queueFamilyPropertiesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(m_PhysicalDevice, &queueFamilyPropertiesCount, nullptr);
        TArray<VkQueueFamilyProperties2> queueFamilyProperties(queueFamilyPropertiesCount);
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


        TArray<VkDeviceQueueCreateInfo> queueCreateInfos;
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


        TArray<const char*> deviceExtensions;
        deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        deviceExtensions.add(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
        deviceExtensions.add(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME);
        deviceExtensions.add(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);

        VkPhysicalDeviceTimelineSemaphoreFeatures timelineSemFeatures = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES};
        timelineSemFeatures.timelineSemaphore = true;

        VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptorBufferFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT };
        descriptorBufferFeatures.pNext = &timelineSemFeatures;
        descriptorBufferFeatures.descriptorBuffer = true;
        descriptorBufferFeatures.descriptorBufferPushDescriptors = true;

        VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParametersFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES };
        shaderDrawParametersFeatures.shaderDrawParameters = true;
        shaderDrawParametersFeatures.pNext = &descriptorBufferFeatures;

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

        if (!m_VulkanFunctions) m_VulkanFunctions = new VmaVulkanFunctions();

        VmaAllocatorCreateInfo allocatorCreateInfo = { 0 };
        allocatorCreateInfo.device = m_Handle;
        allocatorCreateInfo.instance = s_Instance;
        allocatorCreateInfo.physicalDevice = m_PhysicalDevice;
        allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;
        if (VK_FAILED(vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, m_VulkanFunctions)))
        {
            std::wcerr << L"Failed to import vulkan functions pointers to vma!\n";
            return false;
        }

        allocatorCreateInfo.pVulkanFunctions = m_VulkanFunctions;
        if (VK_FAILED(vmaCreateAllocator(&allocatorCreateInfo, &m_Allocator)))
        {
            std::wcerr << L"Failed to create Vulkan allocator!\n";
            return false;
        }

        m_RenderPool = m_RenderQueue.createCommandPool();
        m_ComputePool = m_ComputeQueue.createCommandPool();
        m_CopyPool = m_CopyQueue.createCommandPool();

        FSwapchainDesc swapchainDesc;
        swapchainDesc.device = this;
        swapchainDesc.buffering = deviceDesc.buffering;
        swapchainDesc.format = EFormat::R8G8B8A8_SRGB;
        swapchainDesc.width = deviceDesc.window->getWidth();
        swapchainDesc.height = deviceDesc.window->getHeight();
        swapchainDesc.presentMode = deviceDesc.vSync ? EPresentMode::Fifo : EPresentMode::Immediate;
        if (!m_Swapchain.initialize(swapchainDesc))
        {
            std::wcerr << L"Failed to initialize swapchain!\n";
            return false;
        }

        for (size_t imageIndex = 0; imageIndex < m_Swapchain.getTextureCount(); ++imageIndex)
        {
            m_SubmitSemaphores[imageIndex] = createSemaphore(m_Handle);
        }

        for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        {
            VkFenceCreateInfo fenceCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
            vkCreateFence(m_Handle, &fenceCreateInfo, nullptr, &m_Fences[i]);

            m_CmdBuffers[i].initialize({this, &m_RenderQueue});
            m_CmdBuffers[i].setName(strfmt("Command buffer (frame {})", i));
            m_TextureAvailableSemaphores[i] = createSemaphore(m_Handle);
        }

        VkDescriptorPoolSize poolSizes[]
        {
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 32},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1024},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 64},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 64},
            {VK_DESCRIPTOR_TYPE_SAMPLER, 32},
        };

        VkDescriptorPoolCreateInfo poolCreateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        poolCreateInfo.pPoolSizes = poolSizes;
        poolCreateInfo.poolSizeCount = std::size(poolSizes);
        poolCreateInfo.maxSets = 1024;
        poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
        vkDestroyDescriptorPool(m_Handle, m_DescriptorPool, nullptr);
        if (VK_FAILED(vkCreateDescriptorPool(m_Handle, &poolCreateInfo, nullptr, &m_DescriptorPool)))
            return false;


        m_ImmediateExecutor.initialize({this, &m_RenderQueue});
        //if (SLANG_FAILED(slang::createGlobalSession(&m_SlangSession)))
        //    return false;

        m_Window = deviceDesc.window;
        m_Window->resizedEvent.bind([this](uint32_t, uint32_t) { m_Swapchain.invalidate(); });

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

        FString infoString;
        infoString.append(strfmt("Using Vulkan 1.4\n"));
        infoString.append(strfmt("    Device: {}\n", properties.deviceName));
        infoString.append(strfmt("Successfully initialized render device!"));
        std::cout << infoString << std::endl;

        s_DeviceCount++;
        return true;
    }

    void FGpuDeviceImpl::destroy()
    {
        waitIdle();

        vkDestroyDescriptorPool(m_Handle, m_DescriptorPool, nullptr);
        // I know this shouldn't be there
        //m_SlangSession->release();
        //slang::shutdown();
        m_Window = nullptr;
        m_ImmediateExecutor.destroy();

        for (uint32_t i = 0; i < m_Swapchain.getTextureCount(); ++i)
            vkDestroySemaphore(m_Handle, m_SubmitSemaphores[i], nullptr);

        for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
        {
            vkDestroySemaphore(m_Handle, m_TextureAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_Handle, m_Fences[i], nullptr);
            m_CmdBuffers[i].destroy();
        }

        vkDestroyCommandPool(m_Handle, m_CopyPool, nullptr);
        vkDestroyCommandPool(m_Handle, m_ComputePool, nullptr);
        vkDestroyCommandPool(m_Handle, m_RenderPool, nullptr);

        m_Swapchain.destroy();
        vmaDestroyAllocator(m_Allocator);
        vkDestroySurfaceKHR(s_Instance, m_Surface, nullptr);
        vkDestroyDevice(m_Handle, nullptr);

        s_DeviceCount--;
        if (s_DeviceCount <= 0)
        {
#if defined(LUMA_DEBUG)
            vkDestroyDebugUtilsMessengerEXT(s_Instance, s_DebugMessenger, nullptr);
#endif
            vkDestroyInstance(s_Instance, nullptr);
        }
    }

    bool FGpuDeviceImpl::beginFrame()
    {
        if (!m_Window) return false;
        if (!m_Window->isAvailable()) return false;

        if (!m_Swapchain.isValid())
        {
            waitIdle();

            m_Swapchain.resize(m_Window->getWidth(), m_Window->getHeight());
            m_FrameIndex = 0;
            return false;
        }

        const VkFence fence = m_Fences[m_FrameIndex];
        vkWaitForFences(m_Handle, 1, &fence, true, FENCE_WAIT_INFINITE);
        vkResetFences(m_Handle, 1, &fence);

        if (!m_Swapchain.acquireNextTexture(m_SwapchainImageIndex, m_TextureAvailableSemaphores[m_FrameIndex]))
        {
            m_Swapchain.invalidate();
            return false;
        }

        FCommandBufferImpl& cmdBuffer = m_CmdBuffers[m_FrameIndex];
        cmdBuffer.begin();

        FTextureBarrier barrier;
        barrier.texture = m_Swapchain.getTexture(m_SwapchainImageIndex);
        barrier.destState = EResourceState::ColorTarget;
        barrier.sourceAccess = EResourceAccessBits::None;
        barrier.destAccess = EResourceAccessBits::ColorTargetWrite;
        cmdBuffer.textureBarriers(barrier);
        return true;
    }

    void FGpuDeviceImpl::endFrame()
    {
        FCommandBufferImpl& cmdBuffer = m_CmdBuffers[m_FrameIndex];

        FTextureBarrier barrier;
        barrier.texture = m_Swapchain.getTexture(m_SwapchainImageIndex);
        barrier.destState = EResourceState::Present;
        barrier.sourceAccess = EResourceAccessBits::ColorTargetWrite;
        barrier.destAccess = EResourceAccessBits::None;
        cmdBuffer.textureBarriers(barrier);
        cmdBuffer.end();

        const VkCommandBuffer cmdBuff[] = { cmdBuffer.getHandle() };
        constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
        submitInfo.pCommandBuffers = cmdBuff;
        submitInfo.commandBufferCount = 1;
        submitInfo.pSignalSemaphores = &m_SubmitSemaphores[m_SwapchainImageIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_TextureAvailableSemaphores[m_FrameIndex];
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitDstStageMask = waitStages;

        vkQueueSubmit(m_RenderQueue.getHandle(), 1, &submitInfo, m_Fences[m_FrameIndex]);
    }

    void FGpuDeviceImpl::present()
    {
        const uint32_t indices[] { m_SwapchainImageIndex };

        VkPresentInfoKHR presentInfo {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
        presentInfo.pSwapchains = m_Swapchain.getHandlePtr();
        presentInfo.swapchainCount = 1;
        presentInfo.pWaitSemaphores = &m_SubmitSemaphores[m_SwapchainImageIndex];
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pImageIndices = indices;
        presentInfo.pResults = nullptr;

        vkQueuePresentKHR(m_RenderQueue.getHandle(), &presentInfo);

        m_FrameIndex = (m_FrameIndex + 1) % NUM_FRAMES_IN_FLIGHT;
    }

    void FGpuDeviceImpl::waitIdle()
    {
        vkDeviceWaitIdle(m_Handle);
    }

    uint32_t FGpuDeviceImpl::getTextureCount() const
    {
        return m_Swapchain.getTextureCount();
    }

    uint32_t FGpuDeviceImpl::getFrameIndex() const
    {
        return m_FrameIndex;
    }

    ISwapchain* FGpuDeviceImpl::getSwapchain()
    {
        return &m_Swapchain;
    }

    IQueue* FGpuDeviceImpl::getRenderQueue()
    {
        return &m_RenderQueue;
    }

    IQueue* FGpuDeviceImpl::getComputeQueue()
    {
        return &m_ComputeQueue;
    }

    IQueue* FGpuDeviceImpl::getCopyQueue()
    {
        return &m_CopyQueue;
    }

    IBuffer* FGpuDeviceImpl::createBuffer(const FBufferDesc& bufferDesc)
    {
        FBufferDesc desc(bufferDesc);
        desc.device = this;
        FBufferImpl* buffer = new FBufferImpl();
        if (!buffer->initialize(desc))
        {
            delete buffer;
            return nullptr;
        }
        return buffer;
    }

    ITexture* FGpuDeviceImpl::createTexture(const FTextureDesc& textureDesc)
    {
        FTextureDesc desc(textureDesc);
        desc.device = this;
        FTextureImpl* texture = new FTextureImpl();
        if (!texture->initialize(desc))
        {
            delete texture;
            return nullptr;
        }
        return texture;
    }

    ITextureView* FGpuDeviceImpl::createTextureView(const FTextureViewDesc& textureViewDesc)
    {
        FTextureViewDesc desc(textureViewDesc);
        desc.device = this;
        FTextureViewImpl* textureView = new FTextureViewImpl();
        if (!textureView->initialize(desc))
        {
            delete textureView;
            return nullptr;
        }
        return textureView;
    }

    IShaderProgram* FGpuDeviceImpl::createShader(const FShaderDesc& shaderDesc)
    {
        return nullptr;
    }

    ICommandBuffer* FGpuDeviceImpl::createCommandBuffer(const FCommandBufferDesc& commandBufferDesc)
    {
        FCommandBufferDesc desc(commandBufferDesc);
        desc.device = this;
        FCommandBufferImpl* cmdBuffer = new FCommandBufferImpl();
        if (!cmdBuffer->initialize(desc))
        {
            delete cmdBuffer;
            return nullptr;
        }
        return cmdBuffer;
    }

    ISampler* FGpuDeviceImpl::createSampler(const FSamplerDesc& samplerDesc)
    {
        FSamplerDesc desc(samplerDesc);
        desc.device = this;
        FSamplerImpl* sampler = new FSamplerImpl();
        if (!sampler->initialize(desc))
        {
            delete sampler;
            return nullptr;
        }
        return sampler;
    }

    IRenderPipeline* FGpuDeviceImpl::createRenderPipeline(const FRenderPipelineDesc& pipelineDesc)
    {
        FRenderPipelineDesc desc(pipelineDesc);
        desc.device = this;
        FRenderPipelineImpl* pipeline = new FRenderPipelineImpl();
        if (!pipeline->initialize(desc))
        {
            delete pipeline;
            return nullptr;
        }
        return pipeline;
    }

    IComputePipeline* FGpuDeviceImpl::createComputePipeline(const FComputePipelineDesc& pipelineDesc)
    {
        FComputePipelineDesc desc(pipelineDesc);
        desc.device = this;
        FComputePipelineImpl* pipeline = new FComputePipelineImpl();
        if (!pipeline->initialize(desc))
        {
            delete pipeline;
            return nullptr;
        }
        return pipeline;
    }

    IFence* FGpuDeviceImpl::createFence(const FFenceDesc& fenceDesc)
    {
        FFenceDesc desc(fenceDesc);
        desc.device = this;
        FFenceImpl* fence = new FFenceImpl();
        if (!fence->initialize(desc))
        {
            delete fence;
            return nullptr;
        }
        return fence;
    }

    ITextureView* FGpuDeviceImpl::getAcquiredSwapchainTextureView()
    {
        return m_Swapchain.getTextureView(m_SwapchainImageIndex);
    }

    void FGpuDeviceImpl::writeSamplerDescriptor(IBuffer* buffer, uint64_t offset, const ISampler* sampler)
    {
        const FSamplerImpl* samplerImpl = static_cast<const FSamplerImpl*>(sampler);
        const VkSampler samplerHandle = samplerImpl->getHandle();

        VkDescriptorGetInfoEXT getInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT };
        getInfo.type = VK_DESCRIPTOR_TYPE_SAMPLER;
        getInfo.data.pSampler = &samplerHandle;

        uint8_t* mappedPtr = buffer->map<uint8_t>();
        vkGetDescriptorEXT(m_Handle, &getInfo, m_DescriptorBufferProperties.samplerDescriptorSize, mappedPtr + offset);
        buffer->unmap(mappedPtr);
    }

    void FGpuDeviceImpl::writeTextureDescriptor(IBuffer* buffer, uint64_t offset, const ITexture* texture, ETextureBindingType bindingType)
    {
        const FTextureViewImpl* textureViewImpl = static_cast<const FTextureViewImpl*>(texture->getTextureView());

        VkDescriptorImageInfo imageInfo;
        imageInfo.imageView = textureViewImpl->getHandle();
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkDescriptorGetInfoEXT getInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT };
        getInfo.type = bindingType == ETextureBindingType::Storage ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        getInfo.data.pSampledImage = &imageInfo;
        getInfo.data.pStorageImage = &imageInfo;

        const auto descriptorSize = bindingType == ETextureBindingType::Sampled ?
        m_DescriptorBufferProperties.sampledImageDescriptorSize
        : m_DescriptorBufferProperties.storageImageDescriptorSize;

        uint8_t* mappedPtr = buffer->map<uint8_t>();
        vkGetDescriptorEXT(m_Handle, &getInfo, descriptorSize, mappedPtr + offset);
        buffer->unmap(mappedPtr);
    }

    static VkDescriptorType getDescriptorType(EBufferBindingType bindingType)
    {
        switch (bindingType)
        {
        case EBufferBindingType::Uniform: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case EBufferBindingType::Storage: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case EBufferBindingType::UniformTexel: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case EBufferBindingType::StorageTexel: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        default: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }
    }

    void FGpuDeviceImpl::writeBufferDescriptor(IBuffer* buffer, uint64_t offset, const IBuffer* bufferResource,
        uint64_t resourceOffset, uint64_t resourceSize, EBufferBindingType bindingType)
    {
        LUMA_ASSERT(resourceOffset + resourceSize <= bufferResource->getSize(), "Size is too big!");

        VkDescriptorAddressInfoEXT addressInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT };
        addressInfo.address = bufferResource->getDeviceAddress() + resourceOffset;
        addressInfo.format = VK_FORMAT_UNDEFINED;
        addressInfo.range = resourceSize;

        VkDescriptorGetInfoEXT getInfo { VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT };
        getInfo.type = getDescriptorType(bindingType);
        getInfo.data.pStorageBuffer = bindingType == EBufferBindingType::Storage ? &addressInfo : nullptr;
        getInfo.data.pStorageTexelBuffer = bindingType == EBufferBindingType::StorageTexel ? &addressInfo : nullptr;
        getInfo.data.pUniformBuffer = bindingType == EBufferBindingType::Uniform ? &addressInfo : nullptr;
        getInfo.data.pUniformTexelBuffer = bindingType == EBufferBindingType::UniformTexel ? &addressInfo : nullptr;

        const auto getDescriptorSize = [this, bindingType]()
        {
            switch (bindingType)
            {
            case EBufferBindingType::Uniform: return m_DescriptorBufferProperties.uniformBufferDescriptorSize;
            case EBufferBindingType::Storage: return m_DescriptorBufferProperties.storageBufferDescriptorSize;
            case EBufferBindingType::UniformTexel: return m_DescriptorBufferProperties.uniformBufferDescriptorSize;
            case EBufferBindingType::StorageTexel: return m_DescriptorBufferProperties.storageBufferDescriptorSize;
            default: return 0u;
            }
        };

        uint8_t* mappedPtr = buffer->map<uint8_t>();
        vkGetDescriptorEXT(m_Handle, &getInfo, getDescriptorSize(), mappedPtr + offset);
        buffer->unmap(mappedPtr);
    }

    VkInstance FGpuDeviceImpl::getInstance()
    {
        return s_Instance;
    }

    VkCommandPool FGpuDeviceImpl::getCommandPool(const EQueueType queueType) const
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

    FImmediateExecutorImpl& FGpuDeviceImpl::getExecutor()
    {
        return m_ImmediateExecutor;
    }
}
