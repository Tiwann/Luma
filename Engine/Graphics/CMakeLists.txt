

set(LUMA_GRAPHICS_SRC
        Source/RHI/RenderDevice.cpp
        Source/RHI/CommandBuffer.cpp
)


set(LUMA_WEBGPU_SRC
        External/webgpu/webgpu.c
        Source/WebGPU/Device.cpp
)

add_library(luma-graphics SHARED ${LUMA_GRAPHICS_SRC})
target_include_directories(luma-graphics PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-graphics PRIVATE LUMA_GRAPHICS INTERFACE LUMA_CLIENT)
set_target_properties(luma-graphics PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-graphics MODULE_NAME GRAPHICS)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-graphics PRIVATE LUMA_BUILD_DLL)
endif ()

if(LUMA_ENGINE_BUILD_WEBGPU)
    target_sources(luma-graphics PRIVATE ${LUMA_WEBGPU_SRC})
    target_compile_definitions(luma-graphics PUBLIC LUMA_BUILD_WEBGPU WGPU_SHARED_LIBRARY)
endif ()