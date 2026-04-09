
set(LUMA_MATH_SRC
        Source/Vector.cpp
        Source/Quaternion.cpp
        Source/Matrix.cpp
        Source/AxisAngle.cpp
        Source/Ray.cpp
        Source/Rect.cpp
        Source/AABB.cpp
)

add_library(luma-math ${LUMA_MATH_SRC})
target_link_libraries(luma-math PRIVATE luma-core)
target_include_directories(luma-math PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-math PRIVATE LUMA_MATH INTERFACE LUMA_CLIENT)
set_target_properties(luma-math PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-math MODULE_NAME MATH)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-math PRIVATE LUMA_BUILD_DLL)
endif ()