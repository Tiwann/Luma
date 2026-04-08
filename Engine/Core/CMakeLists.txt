

set(LUMA_CORE_SRC
        Source/Module.cpp
        Source/ModuleManager.cpp
        Source/RTTI.cpp
)

add_library(luma-core ${LUMA_CORE_SRC})
target_include_directories(luma-core PRIVATE Source Include/Luma PUBLIC Include)
target_compile_definitions(luma-core PRIVATE LUMA_CORE INTERFACE LUMA_CLIENT)
set_target_properties(luma-core PROPERTIES CXX_STANDARD 26)
luma_generate_export_header(TARGET luma-core MODULE_NAME CORE)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(luma-core PRIVATE LUMA_BUILD_DLL)
endif ()