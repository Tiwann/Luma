cmake_minimum_required(VERSION 4.0)
project(luma)

include(CMake/LumaHelpers.cmake)

option(LUMA_ENGINE_BUILD_EXAMPLES "Build the example targets" ON)

option(LUMA_ENGINE_BUILD_D3D12 "Build the engine with D3D12 backend" OFF)
option(LUMA_ENGINE_BUILD_VULKAN "Build the engine with Vulkan backend" OFF)
option(LUMA_ENGINE_BUILD_OPENGL "Build the engine with OpenGL backend" OFF)
option(LUMA_ENGINE_BUILD_WEBGPU "Build the engine with WebGPU backend" OFF)
option(LUMA_ENGINE_BUILD_NVN "Build the engine with NVN backend" OFF)
option(LUMA_ENGINE_BUILD_DEKO3D "Build the engine with Deko3D backend" OFF)
option(LUMA_ENGINE_BUILD_GNM "Build the engine with GNM backend" OFF)

option(LUMA_ENGINE_BUILD_GRAPHICS "Build the engine with graphics module" ON)
option(LUMA_ENGINE_BUILD_AUDIO "Build the engine with audio module" ON)

add_subdirectory(Engine/Core)
if(LUMA_ENGINE_BUILD_GRAPHICS)
add_subdirectory(Engine/Graphics)
endif ()

if(LUMA_ENGINE_BUILD_AUDIO)
add_subdirectory(Engine/Audio)
endif ()

if(LUMA_ENGINE_BUILD_EXAMPLES)
add_subdirectory(Example/HelloWorld)
endif ()