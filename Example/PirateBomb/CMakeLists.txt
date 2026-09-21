
add_executable(luma-pirate-bomb Main.cpp Player.cpp FullscreenRenderPass.cpp)
target_link_libraries(luma-pirate-bomb PRIVATE luma::core luma::graphics luma::physics)
set_target_properties(luma-pirate-bomb PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-pirate-bomb PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)