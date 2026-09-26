#include "Luma/Vulkan/Device.h"
#include "Luma/Vulkan/CommandBuffer.h"
#include "Luma/Vulkan/Fence.h"
#include "Luma/Vulkan/Sampler.h"
#include "Luma/Vulkan/Buffer.h"
#include "Luma/Vulkan/Texture.h"
#include "Luma/Vulkan/TextureView.h"
#include "Luma/Vulkan/Shader.h"
#include "Luma/Vulkan/ComputePipeline.h"
#include "Luma/Vulkan/RenderPipeline.h"
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

#ifndef VK_LAYER_RENDERDOC_CAPTURE_NAME
#define VK_LAYER_RENDERDOC_CAPTURE_NAME "VK_LAYER_RENDERDOC_Capture"
#endif



namespace Luma::Vulkan
{
    StringView vkResultToString(VkResult result)
    {
        switch (result)
        {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
        default: return "VK_UNKNOWN_RESULT";
        }
    }

    void printVkResult(VkResult result)
    {
        std::cerr << "VkResult: " << vkResultToString(result) << " (" << result << ")";
    }

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

    DeviceType Device::getDeviceType()
    {
        return DeviceType::Vulkan;
    }

    bool Device::initialize(const RHI::DeviceDesc& deviceDesc)
    {
        if (!deviceDesc.window)
        {
            std::wcerr << L"Failed to initialize render device: Invalid window!" << std::endl;
            return false;
        }

        if (deviceDesc.buffering == SwapchainBuffering::None)
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

            Array<const char*> layers;
            layers.add(VK_LAYER_KHRONOS_VALIDATION_NAME);

            Array<const char*> extensions;
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

            if (!s_DebugMessenger)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
                debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
                debugMessengerCreateInfo.pfnUserCallback = messageCallback;
                if (vkCreateDebugUtilsMessengerEXT(s_Instance, &debugMessengerCreateInfo, nullptr, &s_DebugMessenger) != VK_SUCCESS)
                    return false;
            }
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


        if (DesktopWindow* window = dynamic_cast<DesktopWindow*>(deviceDesc.window))
        {
            if (VK_FAILED(glfwCreateWindowSurface(s_Instance, window->getHandle(), nullptr, &m_Surface)))
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
                m_RenderQueue.setQueueType(QueueType::Render);
            }

            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & (VK_QUEUE_COMPUTE_BIT))
            {
                m_ComputeQueue.setIndex(i);
                m_ComputeQueue.setQueueType(QueueType::Compute);
            }

            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & (VK_QUEUE_TRANSFER_BIT))
            {
                m_CopyQueue.setIndex(i);
                m_CopyQueue.setQueueType(QueueType::Copy);
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
        //deviceExtensions.add(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME);
        deviceExtensions.add(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);

        VkPhysicalDeviceTimelineSemaphoreFeatures timelineSemFeatures = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES};
        timelineSemFeatures.timelineSemaphore = true;

        VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptorBufferFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT };
        descriptorBufferFeatures.pNext = &timelineSemFeatures;
        descriptorBufferFeatures.descriptorBuffer = true;
        descriptorBufferFeatures.descriptorBufferPushDescriptors = true;

        VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParametersFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES };
        shaderDrawParametersFeatures.shaderDrawParameters = true;
        shaderDrawParametersFeatures.pNext = &timelineSemFeatures;

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
        VkResult deviceResult = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Handle);
        if (VK_FAILED(deviceResult))
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

        RHI::SwapchainDesc swapchainDesc;
        swapchainDesc.device = this;
        swapchainDesc.buffering = deviceDesc.buffering;
        swapchainDesc.format = Format::R8G8B8A8_SRGB;
        swapchainDesc.width = deviceDesc.window->getWidth();
        swapchainDesc.height = deviceDesc.window->getHeight();
        swapchainDesc.presentMode = deviceDesc.vSync ? PresentMode::Fifo : PresentMode::Immediate;
        if (!m_Swapchain.initialize(swapchainDesc))
        {
            std::wcerr << L"Failed to initialize swapchain!\n";
            return false;
        }

        for (size_t imageIndex = 0; imageIndex < m_Swapchain.getTextureCount(); ++imageIndex)
        {
            m_SubmitSemaphores[imageIndex] = createSemaphore(m_Handle);
        }

        for (uint32_t i = 0; i < RHI::NUM_FRAMES_IN_FLIGHT; i++)
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

        m_Window = deviceDesc.window;
        m_Window->resizedEvent.bind([this](uint32_t, uint32_t) { m_Swapchain.invalidate(); });

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &properties);

        String infoString;
        infoString.append(strfmt("Initialized Device (Vulkan Backend)\n"));
        infoString.append(strfmt("Device: {}\n", properties.deviceName));
        infoString.append(strfmt("Enabled extensions:\n", properties.deviceName));
        for (const auto& extension : deviceExtensions)
            infoString.append(strfmt("    {}\n", extension));
        std::cout << infoString << std::endl;

        s_DeviceCount++;
        return true;
    }

    void Device::destroy()
    {
        waitIdle();

        vkDestroyDescriptorPool(m_Handle, m_DescriptorPool, nullptr);
        m_Window = nullptr;

        for (uint32_t i = 0; i < m_Swapchain.getTextureCount(); ++i)
            vkDestroySemaphore(m_Handle, m_SubmitSemaphores[i], nullptr);

        for (uint32_t i = 0; i < RHI::NUM_FRAMES_IN_FLIGHT; ++i)
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
            vkDestroyDebugUtilsMessengerEXT(s_Instance, s_DebugMessenger, nullptr);
            vkDestroyInstance(s_Instance, nullptr);
        }
    }

    bool Device::beginFrame()
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
        vkWaitForFences(m_Handle, 1, &fence, true, RHI::FENCE_WAIT_INFINITE);
        vkResetFences(m_Handle, 1, &fence);

        if (!m_Swapchain.acquireNextTexture(m_SwapchainImageIndex, m_TextureAvailableSemaphores[m_FrameIndex]))
        {
            m_Swapchain.invalidate();
            return false;
        }

        CommandBuffer& cmdBuffer = m_CmdBuffers[m_FrameIndex];
        cmdBuffer.begin();

        TextureBarrier barrier;
        barrier.texture = m_Swapchain.getTexture(m_SwapchainImageIndex);
        barrier.destState = ResourceState::ColorTarget;
        barrier.sourceAccess = ResourceAccess::None;
        barrier.destAccess = ResourceAccess::ColorTargetWrite;
        cmdBuffer.textureBarriers(barrier);
        return true;
    }

    void Device::endFrame()
    {
        CommandBuffer& cmdBuffer = m_CmdBuffers[m_FrameIndex];

        TextureBarrier barrier;
        barrier.texture = m_Swapchain.getTexture(m_SwapchainImageIndex);
        barrier.destState = ResourceState::Present;
        barrier.sourceAccess = ResourceAccess::ColorTargetWrite;
        barrier.destAccess = ResourceAccess::None;
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

    void Device::present()
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

        m_FrameIndex = (m_FrameIndex + 1) % RHI::NUM_FRAMES_IN_FLIGHT;
    }

    void Device::waitIdle()
    {
        vkDeviceWaitIdle(m_Handle);
    }

    uint32_t Device::getTextureCount() const
    {
        return m_Swapchain.getTextureCount();
    }

    uint32_t Device::getFrameIndex() const
    {
        return m_FrameIndex;
    }

    RHI::Swapchain* Device::getSwapchain()
    {
        return &m_Swapchain;
    }

    RHI::Queue* Device::getRenderQueue()
    {
        return &m_RenderQueue;
    }

    RHI::Queue* Device::getComputeQueue()
    {
        return &m_ComputeQueue;
    }

    RHI::Queue* Device::getCopyQueue()
    {
        return &m_CopyQueue;
    }

    RHI::Buffer* Device::createBuffer(const RHI::BufferDesc& bufferDesc)
    {
        RHI::BufferDesc desc(bufferDesc);
        desc.device = this;
        Buffer* buffer = new Buffer();
        if (!buffer->initialize(desc))
        {
            delete buffer;
            return nullptr;
        }
        return buffer;
    }

    RHI::Texture* Device::createTexture(const RHI::TextureDesc& textureDesc)
    {
        RHI::TextureDesc desc(textureDesc);
        desc.device = this;
        Texture* texture = new Texture();
        if (!texture->initialize(desc))
        {
            delete texture;
            return nullptr;
        }
        return texture;
    }

    RHI::TextureView* Device::createTextureView(const RHI::TextureViewDesc& textureViewDesc)
    {
        RHI::TextureViewDesc desc(textureViewDesc);
        desc.device = this;
        TextureView* textureView = new TextureView();
        if (!textureView->initialize(desc))
        {
            delete textureView;
            return nullptr;
        }
        return textureView;
    }

    RHI::Shader* Device::createShader(const RHI::ShaderDesc& shaderDesc)
    {
        RHI::ShaderDesc desc(shaderDesc);
        desc.device = this;

        Shader* shader = new Shader();
        if (!shader->initialize(desc))
        {
            delete shader;
            return nullptr;
        }

        return shader;
    }

    RHI::CommandBuffer* Device::createCommandBuffer(const RHI::CommandBufferDesc& commandBufferDesc)
    {
        RHI::CommandBufferDesc desc(commandBufferDesc);
        desc.device = this;
        CommandBuffer* cmdBuffer = new CommandBuffer();
        if (!cmdBuffer->initialize(desc))
        {
            delete cmdBuffer;
            return nullptr;
        }
        return cmdBuffer;
    }

    RHI::Sampler* Device::createSampler(const RHI::SamplerDesc& samplerDesc)
    {
        RHI::SamplerDesc desc(samplerDesc);
        desc.device = this;
        Sampler* sampler = new Sampler();
        if (!sampler->initialize(desc))
        {
            delete sampler;
            return nullptr;
        }
        return sampler;
    }

    RHI::RenderPipeline* Device::createRenderPipeline(const RHI::RenderPipelineDesc& pipelineDesc)
    {
        RHI::RenderPipelineDesc desc(pipelineDesc);
        desc.device = this;
        RenderPipeline* pipeline = new RenderPipeline();
        if (!pipeline->initialize(desc))
        {
            delete pipeline;
            return nullptr;
        }
        return pipeline;
    }

    RHI::ComputePipeline* Device::createComputePipeline(const RHI::ComputePipelineDesc& pipelineDesc)
    {
        RHI::ComputePipelineDesc desc(pipelineDesc);
        desc.device = this;
        ComputePipeline* pipeline = new ComputePipeline();
        if (!pipeline->initialize(desc))
        {
            delete pipeline;
            return nullptr;
        }
        return pipeline;
    }

    RHI::Fence* Device::createFence(const RHI::FenceDesc& fenceDesc)
    {
        RHI::FenceDesc desc(fenceDesc);
        desc.device = this;
        Fence* fence = new Fence();
        if (!fence->initialize(desc))
        {
            delete fence;
            return nullptr;
        }
        return fence;
    }

    RHI::TextureView* Device::getAcquiredSwapchainTextureView()
    {
        return m_Swapchain.getTextureView(m_SwapchainImageIndex);
    }

    RHI::Texture* Device::getAcquiredSwapchainTexture()
    {
        return m_Swapchain.getTexture(m_SwapchainImageIndex);
    }

    VkInstance Device::getInstance()
    {
        return s_Instance;
    }

    VkCommandPool Device::getCommandPool(const QueueType queueType) const
    {
        switch (queueType)
        {
        case QueueType::None: return nullptr;
        case QueueType::Render: return m_RenderPool;
        case QueueType::Compute: return m_ComputePool;
        case QueueType::Copy: return m_CopyPool;
        default: return m_RenderPool;
        }
    }
}
