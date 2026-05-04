
add_executable(luma-mesh Main.cpp)
target_link_libraries(luma-mesh PRIVATE luma::core luma::graphics)
set_target_properties(luma-mesh PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-mesh PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)