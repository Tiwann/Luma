cmake_minimum_required(VERSION 4.0)
project(luma LANGUAGES C CXX)

include(CMakeDependentOption)
include(CMake/LumaHelpers.cmake)

option(LUMA_ENGINE_BUILD_EXAMPLES "Build the example targets" ON)

cmake_dependent_option(LUMA_ENGINE_BUILD_D3D12 "Build the engine with D3D12 backend" ON WIN32 OFF)
option(LUMA_ENGINE_BUILD_VULKAN "Build the engine with Vulkan backend" ON)
option(LUMA_ENGINE_BUILD_OPENGL "Build the engine with OpenGL backend" OFF)
option(LUMA_ENGINE_BUILD_WEBGPU "Build the engine with WebGPU backend" OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_NVN "Build the engine with NVN backend" ON SWITCH OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_DEKO3D "Build the engine with Deko3D backend" ON SWITCH OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_GNM "Build the engine with GNM backend" ON PS5 OFF)

if(LUMA_ENGINE_BUILD_VULKAN)
    include(CMake/Vulkan.cmake)
endif ()

add_library(luma-natvis INTERFACE LumaEngine.natvis)
set(LUMA_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine)

add_subdirectory(Vendor)
add_subdirectory(Engine/Core)
add_subdirectory(Engine/Graphics)
add_subdirectory(Engine/Audio)

if(LUMA_ENGINE_BUILD_EXAMPLES)
add_subdirectory(Example/HelloTriangle)
endif ()

