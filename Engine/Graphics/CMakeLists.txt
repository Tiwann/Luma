

set(LUMA_GRAPHICS_SRC
        Source/Rendering/Device.cpp
        Source/Rendering/Swapchain.cpp
        Source/Rendering/CommandBuffer.cpp
        Source/Rendering/Sampler.cpp
        Source/Rendering/Camera.cpp
        Source/Rendering/ComponentMapping.cpp
        Source/Rendering/BufferUtils.cpp
        Source/Rendering/InputLayout.cpp
        #Source/Rendering/GraphicsPipelineCache.cpp
        Source/Rendering/Renderer2D.cpp
        Source/Rendering/TextureUtils.cpp
        Source/Rendering/ResourceBarrier.cpp
        Source/Rendering/Viewport.cpp
        Source/Rendering/BindingGroup.cpp
        Source/Rendering/Shader.cpp
        #Source/Rendering/SceneRenderer.cpp
        #Source/Utility/SlangCommon.cpp
        Source/Utility/SpirvReflectCommon.cpp
        Source/Asset/StaticMesh.cpp
        Source/Components/StaticMeshRenderer.cpp
)

set(LUMA_VULKAN_SRC
        Source/Vulkan/VulkanMemoryAllocator.cpp
        Source/Vulkan/VulkanUtils.cpp
        Source/Vulkan/Device.cpp
        Source/Vulkan/Sampler.cpp
        Source/Vulkan/Swapchain.cpp
        Source/Vulkan/CommandBuffer.cpp
        Source/Vulkan/Queue.cpp
        Source/Vulkan/Fence.cpp
        Source/Vulkan/Buffer.cpp
        Source/Vulkan/Texture.cpp
        Source/Vulkan/TextureView.cpp
        Source/Vulkan/ImmediateExecutor.cpp
        Source/Vulkan/Shader.cpp
        Source/Vulkan/ComputePipeline.cpp
        Source/Vulkan/RenderPipeline.cpp
        Source/Vulkan/BindingGroup.cpp
)


set(LUMA_D3D12_SRC
        Source/D3D12/GPUDeviceImpl.cpp
        Source/D3D12/Buffer.cpp
        Source/D3D12/SwapchainImpl.cpp
        Source/D3D12/QueueImpl.cpp
        Source/D3D12/CommandBufferImpl.cpp
        Source/D3D12/FenceImpl.cpp
)

set(LUMA_WEBGPU_SRC
        Source/WebGPU/GPUDeviceImpl.cpp
        Source/WebGPU/QueueImpl.cpp
        Source/WebGPU/SwapchainImpl.cpp
        Source/WebGPU/TextureImpl.cpp
        Source/WebGPU/TextureViewImpl.cpp
)

set(LUMA_DEKO3D_SRC
    Source/Deko3D/GPUDeviceImpl.cpp
    Source/Deko3D/SwapchainImpl.cpp
    Source/Deko3D/QueueImpl.cpp
    Source/Deko3D/CommandBufferImpl.cpp
    Source/Deko3D/BufferImpl.cpp
)

set(LUMA_OPENGL_SRC
        Source/OpenGL/GPUDeviceImpl.cpp
        Source/OpenGL/BufferImpl.cpp
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
    target_link_libraries(luma-graphics PRIVATE luma::webgpu)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_WEBGPU)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_sources(luma-graphics PRIVATE ${LUMA_VULKAN_SRC})
    target_link_directories(luma-graphics PRIVATE ${VULKAN_SDK_INC_DIR})
    target_link_libraries(luma-graphics PRIVATE glfw volk::volk Vulkan::Headers GPUOpen::VulkanMemoryAllocator)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_VULKAN)
endif ()

if(LUMA_ENGINE_BUILD_DEKO3D)
    target_sources(luma-graphics PRIVATE ${LUMA_DEKO3D_SRC})
    target_link_libraries(luma-graphics PRIVATE deko3d)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_DEKO3D)
endif ()

if(LUMA_ENGINE_BUILD_D3D12)
    target_sources(luma-graphics PRIVATE ${LUMA_D3D12_SRC})
    target_link_libraries(luma-graphics PRIVATE glfw Microsoft::DirectX-Headers D3D12MemoryAllocator d3d12 dxgi dxguid)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_D3D12)
endif ()

if(LUMA_ENGINE_BUILD_OPENGL)
    target_sources(luma-graphics PRIVATE ${LUMA_OPENGL_SRC})
    target_link_libraries(luma-graphics PRIVATE glfw luma::glad opengl32)
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_OPENGL)
endif ()