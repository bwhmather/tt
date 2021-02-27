project(
  'tt', 'cpp',
  default_options: [
    'cpp_std=c++17',
    'cpp_eh=none',
    'cpp_rtti=false',
  ],
)

cc = meson.get_compiler('cpp')

m_dep = cc.find_library('m', required : false)
glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
libpng_dep = dependency('libpng')

dependencies = [m_dep, glfw_dep, glew_dep, libpng_dep]

includes = include_directories('include')


### Library ###

lib_sources = [
  'src/tt-entities.cpp',
  'src/tt-renderer.cpp',
  'src/tt-texture.cpp',
  'src/tt-error.cpp',
  'src/tt-component-job.cpp',
  'src/tt-component-move-to-target.cpp',
  'src/tt-component-position.cpp',
  'src/tt-component-sprite.cpp',
  'src/tt-component-target.cpp',
  'src/tt-system-move-to-target.cpp',
  'src/tt-system-sprites.cpp',
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
  'src/main.cpp',
  include_directories : includes,
  dependencies : dependencies,
  link_with : lib,
  install : true,
)


### Tests ###

test_includes = include_directories('include', 'tests')

test_suites = {
  'tt-storage-vector': [
    'no-default-constructor',
  ],
  'tt-entities': [
    'new',
    'release',
    'realloc-free-list',
    'iterate',
  ],
  'tt-component-sprite': [
    'basic',
  ],
}

foreach suite, tests : test_suites
  foreach test_name : tests
    test_exe = executable(
      'test-' + suite + '-' + test_name,
      'tests/' + suite + '/test-' + test_name + '.cpp',
      include_directories : test_includes,
      link_with : lib,
      install : false,
    )
    test(suite + '-' + test_name, test_exe, timeout: 120)
  endforeach
endforeach
