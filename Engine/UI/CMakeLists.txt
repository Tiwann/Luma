
set(LUMA_UI_SRC
    Source/Rendering/ImGuiRenderer.cpp
)

set(LUMA_UI_VULKAN_SRC
    Source/Vulkan/ImguiRendererImpl.cpp
)

add_library(luma-ui ${LUMA_UI_SRC})
add_library(luma::ui ALIAS luma-ui)
target_link_libraries(luma-ui PRIVATE luma::rgfw luma::core luma::graphics PUBLIC luma::imgui)
target_include_directories(luma-ui PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-ui PRIVATE LUMA_INPUT)
set_target_properties(luma-ui PROPERTIES CXX_STANDARD 26 FOLDER luma-engine)
luma_generate_export_header(TARGET luma-ui MODULE_NAME UI)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-ui PRIVATE LUMA_BUILD_DLL)
endif ()

if(LUMA_ENGINE_BUILD_VULKAN)
    target_sources(luma-ui PRIVATE ${LUMA_UI_VULKAN_SRC})
endif ()