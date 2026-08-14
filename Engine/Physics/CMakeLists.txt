
set(LUMA_PHYSICS_SRC
    Source/PhysicsWorld.cpp
    Source/PhysicsBody.cpp
)

add_library(luma-physics ${LUMA_PHYSICS_SRC})
add_library(luma::physics ALIAS luma-physics)
target_link_libraries(luma-physics PRIVATE luma-core PUBLIC box3d::box3d)
target_include_directories(luma-physics PRIVATE Source External PUBLIC Include)
target_compile_definitions(luma-physics PRIVATE LUMA_PHYSICS $<$<BOOL:${BUILD_SHARED_LIBS}>:LUMA_BUILD_DLL>)
set_target_properties(luma-physics PROPERTIES CXX_STANDARD 26 FOLDER luma-engine)
luma_generate_export_header(TARGET luma-physics MODULE_NAME PHYSICS)