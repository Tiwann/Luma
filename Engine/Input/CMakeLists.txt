
set(LUMA_INPUT_SRC
    Source/Platform/DesktopInputProvider.cpp
)

add_library(luma-input ${LUMA_INPUT_SRC})
target_link_libraries(luma-input PRIVATE luma-core RGFW::RGFW)
target_include_directories(luma-input PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-input PRIVATE LUMA_INPUT INTERFACE LUMA_CLIENT)
set_target_properties(luma-input PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-input MODULE_NAME INPUT)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-input PRIVATE LUMA_BUILD_DLL)
endif ()