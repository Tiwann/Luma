
add_executable(luma-texture Main.cpp ArgParse.cpp)
target_link_libraries(luma-texture PRIVATE luma::core luma::graphics luma::ui)
set_target_properties(luma-texture PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-texture PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)