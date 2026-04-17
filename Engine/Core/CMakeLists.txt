

set(LUMA_CORE_SRC
        Source/Math/AABB.cpp
        Source/Math/AxisAngle.cpp
        Source/Math/Matrix.cpp
        Source/Math/Quaternion.cpp
        Source/Math/Ray.cpp
        Source/Math/Rect.cpp
        Source/Math/Transform.cpp
        Source/Math/Vector.cpp
        Include/Luma/Math/MatrixTransform.hxx
        Include/Luma/Math/MatrixProjection.hxx
        Source/Modules/ModuleManager.cpp
        Source/Runtime/RTTI.cpp
        ##Source/Runtime/Application.cpp
        Source/Memory/Memory.cpp
        Source/Memory/Allocator.cpp
        Include/Luma/Containers/Array.h
)

set(LUMA_DESKTOP_SRC
        Source/Runtime/DesktopWindow.cpp
)



add_library(luma-core ${LUMA_CORE_SRC})
target_include_directories(luma-core PRIVATE Source PUBLIC Include External)
target_compile_definitions(luma-core PRIVATE LUMA_CORE INTERFACE LUMA_CLIENT)
set_target_properties(luma-core PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-core MODULE_NAME CORE)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-core PRIVATE LUMA_BUILD_DLL)
endif ()

if(WIN32 OR APPLE OR LINUX)
target_sources(luma-core PRIVATE ${LUMA_DESKTOP_SRC})
target_link_libraries(luma-core PRIVATE RGFW::RGFW)
endif ()


if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(luma-core PUBLIC LUMA_DEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    target_compile_definitions(luma-core PUBLIC LUMA_DEV)
elseif (CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    target_compile_definitions(luma-core PUBLIC LUMA_RELEASE)
endif ()

if (WIN32)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_WINDOWS UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)
elseif (LINUX)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_LINUX)
elseif (MACOS)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_MACOS)
elseif(SWITCH)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_SWITCH)
    target_link_libraries(luma-core PUBLIC libnx)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_compile_definitions(luma-core PUBLIC LUMA_BUILD_VULKAN)
endif ()