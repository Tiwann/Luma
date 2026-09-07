add_library(luma-glad STATIC)
add_library(luma::glad ALIAS luma-glad)
target_include_directories(luma-glad PUBLIC include)
target_sources(luma-glad PRIVATE src/glad.c)