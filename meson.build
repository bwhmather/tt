project('tt', 'c')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required : false)
glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')

dependencies = [m_dep, glfw_dep, glew_dep]

includes = include_directories('include')


### Library ###

lib_sources = [
  'src/tt-entities.c',
  'src/tt-entity-set.c',
  'src/tt-entity-iter.c',
  'src/tt-renderer.c',
  'src/tt-storage-vector.c',
  'src/tt-component-position.c',
]

lib = both_libraries(
  'tt', lib_sources,
  include_directories : includes,
  dependencies : dependencies,
  install : true,
)


### Game ###

test_exe = executable(
  'tt',
  'src/main.c',
  include_directories : includes,
  dependencies : dependencies,
  link_with : lib,
  install : true,
)


### Tests ###

test_includes = include_directories('include', 'tests')

test_names = [
  'tt-test-entities-new',
  'tt-test-entities-release',
  'tt-test-entities-realloc-free-list',
  'tt-test-storage-vector-basic',
  'tt-test-storage-vector-realloc',
  'tt-test-storage-vector-removal',
]

foreach test_name : test_names
  test_exe = executable(
    test_name, 'tests/' + test_name + '.c',
    include_directories : test_includes,
    link_with : lib,
    install : false,
  )
  test(test_name, test_exe, timeout: 120)
endforeach
