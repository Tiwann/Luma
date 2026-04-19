

set(LUMA_GRAPHICS_SRC
        Source/Rendering/Camera.cpp
        Source/Rendering/RenderDevice.cpp
        Source/Rendering/CommandBuffer.cpp
        Source/Rendering/Swapchain.cpp
        Source/Rendering/Sampler.cpp
        Source/Rendering/ComponentMapping.cpp
)

set(LUMA_VULKAN_SRC
        Source/Vulkan/VulkanMemoryAllocator.cpp
        Source/Vulkan/RenderDeviceImpl.cpp
        Source/Vulkan/SamplerImpl.cpp
        Source/Vulkan/SwapchainImpl.cpp
        Source/Vulkan/CommandBufferImpl.cpp
        Source/Vulkan/QueueImpl.cpp
        Source/Vulkan/FenceImpl.cpp
        Source/Vulkan/BufferImpl.cpp
        Source/Vulkan/TextureImpl.cpp
        Source/Vulkan/TextureViewImpl.cpp
        Source/Vulkan/ImmediateExecutorImpl.cpp
        Source/Vulkan/BindingSetLayoutImpl.cpp
        Source/Vulkan/BindingSetImpl.cpp
)

set(LUMA_WEBGPU_SRC
        External/webgpu/webgpu.c
        Source/WebGPU/Device.cpp
)

add_library(luma-graphics ${LUMA_GRAPHICS_SRC})
target_include_directories(luma-graphics PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-graphics PRIVATE LUMA_GRAPHICS INTERFACE LUMA_CLIENT)
target_link_libraries(luma-graphics PRIVATE luma-core RGFW::RGFW)
set_target_properties(luma-graphics PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-graphics MODULE_NAME GRAPHICS)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-graphics PRIVATE LUMA_BUILD_DLL)
endif ()

if(LUMA_ENGINE_BUILD_WEBGPU)
    target_sources(luma-graphics PRIVATE ${LUMA_WEBGPU_SRC})
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_WEBGPU WGPU_SHARED_LIBRARY)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_sources(luma-graphics PRIVATE ${LUMA_VULKAN_SRC})
    target_link_directories(luma-graphics PRIVATE ${VULKAN_SDK_INC_DIR})
    target_link_libraries(luma-graphics PRIVATE Vulkan::volk)
endif ()