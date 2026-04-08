
add_executable(LumaEngine-HelloWorld Main.cpp)
target_link_libraries(LumaEngine-HelloWorld PRIVATE luma-core luma-audio luma-graphics)
set_target_properties(LumaEngine-HelloWorld PROPERTIES CXX_STANDARD 26)