

set(LUMA_GRAPHICS_SRC
        Source/Rendering/Camera.cpp
        Source/Rendering/RenderDevice.cpp
        Source/Rendering/CommandBuffer.cpp
        Source/Rendering/Swapchain.cpp
        Source/Rendering/Sampler.cpp
        Source/Rendering/ComponentMapping.cpp
        Source/Rendering/BufferUtils.cpp
        Source/Rendering/InputLayout.cpp
        #Source/Rendering/GraphicsPipelineCache.cpp
        Source/Rendering/Renderer2D.cpp
        Source/Rendering/TextureUtils.cpp
        Source/Rendering/ResourceBarrier.cpp
        #Source/Rendering/SceneRenderer.cpp
        #Source/Utility/SlangCommon.cpp
        Source/Utility/SpirvReflectCommon.cpp
        Source/Asset/StaticMesh.cpp
        Source/Asset/Material.cpp
        Source/Components/StaticMeshRenderer.cpp
)

set(LUMA_VULKAN_SRC
        Source/Vulkan/VulkanMemoryAllocator.cpp
        Source/Vulkan/VulkanUtils.cpp
        Source/Vulkan/RenderDeviceImpl.cpp
        Source/Vulkan/SamplerImpl.cpp
        Source/Vulkan/SwapchainImpl.cpp
        Source/Vulkan/CommandBufferImpl.cpp
        Source/Vulkan/QueueImpl.cpp
        Source/Vulkan/SemaphoreImpl.cpp
        Source/Vulkan/FenceImpl.cpp
        Source/Vulkan/BufferImpl.cpp
        Source/Vulkan/TextureImpl.cpp
        Source/Vulkan/TextureViewImpl.cpp
        Source/Vulkan/ImmediateExecutorImpl.cpp
        Source/Vulkan/BindingSetLayoutImpl.cpp
        Source/Vulkan/BindingSetImpl.cpp
        Source/Vulkan/ShaderImpl.cpp
        Source/Vulkan/ComputePipelineImpl.cpp
        Source/Vulkan/RenderPipelineImpl.cpp
)


set(LUMA_D3D12_SRC
        Source/D3D12/RenderDeviceImpl.cpp
        Source/D3D12/Buffer.cpp
        Source/D3D12/SwapchainImpl.cpp
        Source/D3D12/QueueImpl.cpp
        Source/D3D12/CommandBufferImpl.cpp
        Source/D3D12/FenceImpl.cpp
)

set(LUMA_WEBGPU_SRC
        External/webgpu/webgpu.c
        Source/WebGPU/Device.cpp
)

set(LUMA_DEKO3D_SRC
    Source/Deko3D/RenderDeviceImpl.cpp
    Source/Deko3D/SwapchainImpl.cpp
    Source/Deko3D/QueueImpl.cpp
    Source/Deko3D/CommandBufferImpl.cpp
    Source/Deko3D/BufferImpl.cpp
)

add_library(luma-graphics ${LUMA_GRAPHICS_SRC})
add_library(luma::graphics ALIAS luma-graphics)
target_link_libraries(luma-graphics PRIVATE luma::core spirv-reflect-static assimp::assimp stb::image)
target_include_directories(luma-graphics PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-graphics PRIVATE LUMA_GRAPHICS $<$<BOOL:${BUILD_SHARED_LIBS}>:LUMA_BUILD_DLL>)
set_target_properties(luma-graphics PROPERTIES CXX_STANDARD 26 FOLDER luma-engine)
luma_generate_export_header(TARGET luma-graphics MODULE_NAME GRAPHICS)

if(LUMA_ENGINE_BUILD_WEBGPU)
    target_sources(luma-graphics PRIVATE ${LUMA_WEBGPU_SRC})
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_WEBGPU WGPU_SHARED_LIBRARY)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_sources(luma-graphics PRIVATE ${LUMA_VULKAN_SRC})
    target_link_directories(luma-graphics PRIVATE ${VULKAN_SDK_INC_DIR})
    target_link_libraries(luma-graphics PRIVATE luma::rgfw volk::volk volk::volk_headers Vulkan::Headers GPUOpen::VulkanMemoryAllocator)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_VULKAN)
endif ()

if(LUMA_ENGINE_BUILD_DEKO3D)
    target_sources(luma-graphics PRIVATE ${LUMA_DEKO3D_SRC})
    target_link_libraries(luma-graphics PRIVATE deko3d)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_DEKO3D)
endif ()

if(LUMA_ENGINE_BUILD_D3D12)
    target_sources(luma-graphics PRIVATE ${LUMA_D3D12_SRC})
    target_link_libraries(luma-graphics PRIVATE luma::rgfw Microsoft::DirectX-Headers D3D12MemoryAllocator d3d12 dxgi dxguid)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_D3D12)
endif ()