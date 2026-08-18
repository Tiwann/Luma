
set(LUMA_INPUT_SRC
    Source/Input/Immediate.cpp

)

add_library(luma-input ${LUMA_INPUT_SRC})
add_library(luma::input ALIAS luma-input)
target_link_libraries(luma-input PRIVATE luma::core luma::rgfw)
target_include_directories(luma-input PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-input PRIVATE LUMA_INPUT $<$<BOOL:${BUILD_SHARED_LIBS}>:LUMA_BUILD_DLL>)
set_target_properties(luma-input PROPERTIES CXX_STANDARD 26 FOLDER luma-engine)
luma_generate_export_header(TARGET luma-input MODULE_NAME INPUT)