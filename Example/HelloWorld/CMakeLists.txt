
add_executable(LumaEngine-HelloWorld Main.cpp)
target_link_libraries(LumaEngine-HelloWorld PRIVATE luma-core luma-audio luma-graphics)
set_target_properties(LumaEngine-HelloWorld PROPERTIES CXX_STANDARD 26)

add_custom_command(TARGET LumaEngine-HelloWorld POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:LumaEngine-HelloWorld>
        $<TARGET_FILE_DIR:LumaEngine-HelloWorld>
        COMMAND_EXPAND_LISTS
)