

set(LUMA_CORE_SRC
        Source/External/rgfw/rgfw.c
        Source/Runtime/DesktopWindow.cpp
        Source/Math/AABB.cpp
        Source/Math/AxisAngle.cpp
        Source/Math/Matrix.cpp
        Source/Math/Quaternion.cpp
        Source/Math/Ray.cpp
        Source/Math/Rect.cpp
        Source/Math/Transform.cpp
        Source/Math/Vector.cpp
        Source/Math/MatrixTransform.cpp
        Source/Modules/ModuleManager.cpp
        Source/Runtime/RTTI.cpp
)

add_library(luma-core ${LUMA_CORE_SRC})
target_include_directories(luma-core PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-core PRIVATE LUMA_CORE INTERFACE LUMA_CLIENT)
set_target_properties(luma-core PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-core MODULE_NAME CORE)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-core PRIVATE LUMA_BUILD_DLL RGFW_EXPORT INTERFACE RGFW_IMPORT)
endif ()