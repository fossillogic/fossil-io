cmake_minimum_required(VERSION 3.16)

project(FossilIo
    VERSION 1.0.2
    DESCRIPTION "Lightweight, portable input/output stream library"
    LANGUAGES C
    HOMEPAGE_URL "https://github.com/fossillogic/fossil-io"
)

# Mirror Meson defaults
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)

# Same POSIX feature-test macro used by Meson
add_compile_definitions(_POSIX_C_SOURCE=200809L)

# Subdirectory layout mirrors Meson: root -> code -> logic
add_subdirectory(code)