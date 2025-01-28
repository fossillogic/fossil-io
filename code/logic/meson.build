dir = include_directories('.')
cc = meson.get_compiler('c')

fossil_io_lib = library('fossil-io',
    files('parser.c', 'input.c', 'output.c', 'error.c', 'soap.c', 'stream.c'),
    install: true,
    dependencies: cc.find_library('m', required : false),
    include_directories: dir)

fossil_io_dep = declare_dependency(
    link_with: [fossil_io_lib],
    include_directories: dir)
