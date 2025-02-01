# code/logic/CMakeLists.txt

# Define the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Collect all header files in logic/fossil/*
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/fossil/*.h)

# List the source files
set(TEST_CODE
    serialize.c
    parser.c
    error.c
    input.c
    output.c
    soap.c
    stream.c
    keyboard.c
)

# Create the library target
add_library(fossil-io STATIC ${TEST_CODE} ${HEADER_FILES})

# Link the math library
target_link_libraries(fossil-io PUBLIC m)

# Set the library to be installed
install(TARGETS fossil-io
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install the header files
install(FILES ${HEADER_FILES} DESTINATION include/fossil)

# Declare the dependency
add_library(fossil_io_dep INTERFACE)
target_link_libraries(fossil_io_dep INTERFACE fossil-io)
target_include_directories(fossil_io_dep INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})