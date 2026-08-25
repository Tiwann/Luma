
add_executable(luma-triangle Main.cpp)
target_link_libraries(luma-triangle PRIVATE luma-core luma-audio luma-graphics luma-input)
set_target_properties(luma-triangle PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-triangle PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)

add_shader_target(luma-triangle-shaders
        OUTPUT_TYPE spirv dxil
        SOURCES Assets/Shaders/HelloTriangle.slang
        VERTEX vert
        FRAG frag
)