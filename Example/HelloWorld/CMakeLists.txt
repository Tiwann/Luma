
add_executable(luma-triangle Main.cpp)
target_link_libraries(luma-triangle PRIVATE luma-core luma-audio luma-graphics luma-input)
set_target_properties(luma-triangle PROPERTIES CXX_STANDARD 26)
target_compile_definitions(luma-triangle PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}")