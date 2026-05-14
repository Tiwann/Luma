
add_executable(luma-editor
    Source/EditorApplication.cpp
)

target_link_libraries(luma-editor PRIVATE luma::natvis luma::core luma::graphics luma::audio luma::input luma::ui)
set_target_properties(luma-editor PROPERTIES CXX_STANDARD 26 FOLDER luma-editor WIN32_EXECUTABLE $<CONFIG:Release>)
target_compile_definitions(luma-editor PRIVATE LUMA_APPLICATION_DIR="${CMAKE_CURRENT_SOURCE_DIR}" LUMA_CLIENT)