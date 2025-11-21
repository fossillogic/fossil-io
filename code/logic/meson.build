dir = include_directories('.')
cc = meson.get_compiler('c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'c')
add_project_arguments('-D_POSIX_C_SOURCE=200112L', language: 'cpp')

fossil_io_lib = library('fossil_io',
    files(
        'archive.c',
        'input.c',
        'output.c',
        'error.c',
        'soap.c',
        'file.c',
        'dir.c',
        'cstring.c'
    ),
    install: true,
    dependencies: [cc.find_library('m', required: false)],
    include_directories: dir)

fossil_io_dep = declare_dependency(
    link_with: [fossil_io_lib],
    include_directories: dir)

meson.override_dependency('fossil-io', fossil_io_dep)
