
add_executable(luma-pong Source/Main.cpp Source/Game.cpp)
target_link_libraries(luma-pong PRIVATE luma-core luma-audio luma-graphics)
set_target_properties(luma-pong PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-pong PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)