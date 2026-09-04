add_library(luma-webgpu STATIC IMPORTED GLOBAL)
add_library(luma::webgpu ALIAS luma-webgpu)
target_include_directories(luma-webgpu INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(luma-webgpu INTERFACE glfw3webgpu)

if(WIN32)
    target_link_libraries(luma-webgpu INTERFACE Ws2_32)
    set_target_properties(luma-webgpu PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/windows-x64/wgpu_native.lib
            IMPORTED_PDB ${CMAKE_CURRENT_SOURCE_DIR}/bin/windows-x64/wgpu_native.pdb
    )
elseif(APPLE)
    set_target_properties(luma-webgpu PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/macos-arm64/libwgpu_native.a
    )
elseif(UNIX)
    set_target_properties(luma-webgpu PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux-x64/libwgpu_native.a
    )
else()
    message(FATAL_ERROR "luma-webgpu: unsupported platform")
endif()

# wgpu-native commonly requires these system libs to link successfully
if(WIN32)
    target_link_libraries(luma-webgpu INTERFACE d3dcompiler userenv bcrypt ntdll opengl32)
elseif(APPLE)
    target_link_libraries(luma-webgpu INTERFACE
            "-framework Metal" "-framework QuartzCore" "-framework CoreFoundation"
    )
elseif(UNIX)
    target_link_libraries(luma-webgpu INTERFACE dl pthread m X11)
endif()