# code/logic/CMakeLists.txt
# Mirrors code/logic/meson.build

set(FOSSIL_IO_SOURCES
    archive.c
    regex.c
    input.c
    output.c
    error.c
    soap.c
    filesys.c
    cstring.c
    cipher.c
)

add_library(fossil_io ${FOSSIL_IO_SOURCES})
add_library(fossil::io ALIAS fossil_io)

# Include directory is the current dir (code/logic) so that
# #include <fossil/io/...> works, exactly like Meson.
target_include_directories(fossil_io
    PUBLIC
        \( <BUILD_INTERFACE: \){CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# libm when available (same as Meson find_library('m', required: false))
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(fossil_io PRIVATE ${MATH_LIBRARY})
endif()

# Installation (headers keep the fossil/io/ hierarchy)
include(GNUInstallDirs)

install(TARGETS fossil_io
    EXPORT fossil-io-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/fossil/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fossil
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT fossil-io-targets
    FILE fossil-io-targets.cmake
    NAMESPACE fossil::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fossil-io
)

# Config file so consumers can find_package(fossil-io)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/fossil-io-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/fossil-io-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fossil-io
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/fossil-io-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/fossil-io-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/fossil-io-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fossil-io
)