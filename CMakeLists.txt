cmake_minimum_required(VERSION 4.0)
project(luma)

include(CMake/LumaHelpers.cmake)

option(LUMA_ENGINE_BUILD_D3D12 "Build the engine with D3D12 backend" OFF)
option(LUMA_ENGINE_BUILD_VULKAN "Build the engine with Vulkan backend" OFF)
option(LUMA_ENGINE_BUILD_OPENGL "Build the engine with OpenGL backend" OFF)
option(LUMA_ENGINE_BUILD_WEBGPU "Build the engine with WebGPU backend" OFF)
option(LUMA_ENGINE_BUILD_NVN "Build the engine with NVN backend" OFF)
option(LUMA_ENGINE_BUILD_DEKO3D "Build the engine with Deko3D backend" OFF)

add_subdirectory(Engine/Core)
add_subdirectory(Engine/Graphics)
add_subdirectory(Engine/Audio)
add_subdirectory(Engine/Math)
add_subdirectory(Example/HelloWorld)