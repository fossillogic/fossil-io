dir = include_directories('.')
cc = meson.get_compiler('c')

fossil_io_lib = library('fossil_io',
    files('serialize.c', 'parser.c', 'input.c', 'output.c', 'error.c', 'soap.c', 'stream.c', 'cstring.c'),
    install: true,
    dependencies: [cc.find_library('m', required: false)],
    include_directories: dir)

fossil_io_dep = declare_dependency(
    link_with: [fossil_io_lib],
    include_directories: dir)

meson.override_dependency('fossil-io', fossil_io_dep)
