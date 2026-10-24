
add_executable(luma-rtti Main.cpp Components.h Components.cpp)
target_link_libraries(luma-rtti PRIVATE luma-core)
set_target_properties(luma-rtti PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-rtti PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)
