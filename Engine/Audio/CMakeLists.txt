

set(LUMA_AUDIO_SRC
"Source/miniaudio.c"
"Source/AudioDevice.cpp")

add_library(luma-audio ${LUMA_AUDIO_SRC})
target_link_libraries(luma-audio PRIVATE luma-core)
target_include_directories(luma-audio PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-audio PRIVATE LUMA_AUDIO INTERFACE LUMA_CLIENT)
set_target_properties(luma-audio PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-audio MODULE_NAME AUDIO)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-audio PRIVATE LUMA_BUILD_DLL)
endif ()