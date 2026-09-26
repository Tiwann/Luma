
add_executable(luma-skybox Main.cpp SkinnedMesh.cpp)
target_link_libraries(luma-skybox PRIVATE luma::core luma::graphics stb::image assimp::assimp luma::natvis)
set_target_properties(luma-skybox PROPERTIES CXX_STANDARD 26 FOLDER luma-examples)
target_compile_definitions(luma-skybox PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)