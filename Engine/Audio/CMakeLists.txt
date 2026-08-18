
set(LUMA_AUDIO_SRC
        External/miniaudio.c
        Source/AudioDevice.cpp
        Source/AudioClip.cpp
)

add_library(luma-audio ${LUMA_AUDIO_SRC})
add_library(luma::audio ALIAS luma-audio)
target_link_libraries(luma-audio PRIVATE luma-core)
target_include_directories(luma-audio PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-audio PRIVATE LUMA_AUDIO $<$<BOOL:${BUILD_SHARED_LIBS}>:LUMA_BUILD_DLL>)
set_target_properties(luma-audio PROPERTIES CXX_STANDARD 26 FOLDER luma-engine)
luma_generate_export_header(TARGET luma-audio MODULE_NAME AUDIO)