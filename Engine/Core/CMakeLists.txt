

set(LUMA_CORE_SRC
        Include/Luma/Containers/Array.h
        Include/Luma/Containers/Character.h
        Include/Luma/Containers/Lazy.h
        Include/Luma/Containers/String.h
        Include/Luma/Containers/StringConversion.h
        Include/Luma/Containers/StringFormat.h
        Include/Luma/Containers/StringView.h
        Include/Luma/Containers/BufferView.h
        Include/Luma/Containers/HashMap.h
        Include/Luma/Containers/Pair.h
        Include/Luma/Containers/Event.h

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
        Source/Runtime/Path.cpp
        Source/Runtime/DialogFilters.cpp
        Source/Containers/StringFormat.cpp
        ##Source/Runtime/Application.cpp
        Source/Memory/Memory.cpp
        Source/Memory/Allocator.cpp
        Source/Runtime/Window.cpp
)

set(LUMA_CORE_WINDOWS_SRC
        Source/Runtime/DesktopWindow.cpp
        Source/Platform/Windows/StringConversion.cpp
        Source/Platform/Windows/FileDialog.cpp
        Source/Platform/Windows/Time.cpp
        Source/Platform/Windows/FileSystem.cpp
)


add_library(luma-core ${LUMA_CORE_SRC})
target_link_libraries(luma-core PUBLIC luma-natvis)
target_include_directories(luma-core PRIVATE Source PUBLIC Include External)
target_compile_definitions(luma-core PRIVATE LUMA_CORE INTERFACE LUMA_CLIENT PUBLIC LUMA_ENGINE_ROOT_DIR=\"${LUMA_ENGINE_ROOT_DIR}\")
set_target_properties(luma-core PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-core MODULE_NAME CORE)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-core PRIVATE LUMA_BUILD_DLL)
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
    target_sources(luma-core PRIVATE ${LUMA_CORE_WINDOWS_SRC})
    target_link_libraries(luma-core PRIVATE RGFW::RGFW)
elseif (LINUX)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_LINUX)
    target_link_libraries(luma-core PRIVATE RGFW::RGFW)
elseif (MACOS)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_MACOS)
    target_link_libraries(luma-core PRIVATE RGFW::RGFW)
elseif(SWITCH)
    target_compile_definitions(luma-core PUBLIC LUMA_PLATFORM_SWITCH)
    target_link_libraries(luma-core PUBLIC libnx)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_compile_definitions(luma-core PUBLIC LUMA_BUILD_VULKAN)
endif ()