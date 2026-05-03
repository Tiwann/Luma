
add_executable(luma-renderer2d Main.cpp)
target_link_libraries(luma-renderer2d PRIVATE luma-core luma-audio luma-graphics)
set_target_properties(luma-renderer2d PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-renderer2d PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)