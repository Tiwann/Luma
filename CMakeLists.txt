cmake_minimum_required(VERSION 4.0)
project(luma LANGUAGES C CXX)

include(CMakeDependentOption)
include(CMake/LumaHelpers.cmake)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
option(BUILD_TESTING "Build test targets" OFF)
option(LUMA_ENGINE_BUILD_EXAMPLES "Build the example targets" ON)
option(LUMA_ENGINE_BUILD_EDITOR "Build the editor application" ON)

cmake_dependent_option(LUMA_ENGINE_BUILD_D3D12 "Build the engine with D3D12 backend" ON WIN32 OFF)
option(LUMA_ENGINE_BUILD_VULKAN "Build the engine with Vulkan backend" ON)
option(LUMA_ENGINE_BUILD_OPENGL "Build the engine with OpenGL backend" OFF)
option(LUMA_ENGINE_BUILD_WEBGPU "Build the engine with WebGPU backend" OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_NVN "Build the engine with NVN backend" OFF SWITCH OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_DEKO3D "Build the engine with Deko3D backend" ON SWITCH OFF)
cmake_dependent_option(LUMA_ENGINE_BUILD_GNM "Build the engine with GNM backend" ON PS5 OFF)


add_library(luma-natvis INTERFACE LumaEngine.natvis)
add_library(luma::natvis ALIAS luma-natvis)
set_target_properties(luma-natvis PROPERTIES FOLDER luma-engine)

set(LUMA_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(Vendor)
add_subdirectory(Engine/Core)
add_subdirectory(Engine/Graphics)
add_subdirectory(Engine/Audio)
add_subdirectory(Engine/Input)
add_subdirectory(Engine/UI)
add_subdirectory(Engine/Physics)

if(LUMA_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(Example/HelloTriangle)
    add_subdirectory(Example/HelloTexture)
    add_subdirectory(Example/HelloMesh)
    add_subdirectory(Example/Renderer2D)
    add_subdirectory(Example/PongGame)
endif ()

if(LUMA_ENGINE_BUILD_EDITOR)
    add_subdirectory(Editor)
endif ()