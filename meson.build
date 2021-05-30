project(
  'tt', 'cpp', 'c',
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
cglm_dep = dependency('cglm', fallback : ['cglm', 'cglm_dep'])
libpng_dep = dependency('libpng')

dependencies = [m_dep, glfw_dep, glew_dep, cglm_dep, libpng_dep]

includes = include_directories('include')


### Library ###

lib_sources = [
  'src/tt-behaviour.c',
  'src/tt-behaviour-fail.c',
  'src/tt-behaviour-harvest-target.c',
  'src/tt-behaviour-idle.c',
  'src/tt-behaviour-inventory-full.c',
  'src/tt-behaviour-loop.c',
  'src/tt-behaviour-selector.c',
  'src/tt-behaviour-selector-until.c',
  'src/tt-behaviour-select-stockpile.c',
  'src/tt-behaviour-select-tree.c',
  'src/tt-behaviour-sequence.c',
  'src/tt-behaviour-sleep.c',
  'src/tt-behaviour-succeed.c',
  'src/tt-behaviour-walk-to-target.c',
  'src/tt-component-behaviour.cpp',
  'src/tt-component-brain.cpp',
  'src/tt-component-harvestable.cpp',
  'src/tt-component-position.cpp',
  'src/tt-component-sprite.cpp',
  'src/tt-component-target.cpp',
  'src/tt-component-wood.cpp',
  'src/tt-entities.cpp',
  'src/tt-error.c',
  'src/tt-renderer.c',
  'src/tt-resource-camera.c',
  'src/tt-system-ai.c',
  'src/tt-system-behaviour.c',
  'src/tt-system-sprites.c',
  'src/tt-texture.c',
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

test_suites = {
  'tt-entities': [
    'new',
    'release',
    'realloc-free-list',
    'iterate',
  ],
  'tt-behaviours': [
    'loop-sleep',
  ],
}

foreach suite, tests : test_suites
  foreach test_name : tests
    test_exe = executable(
      'test-' + suite + '-' + test_name,
      'tests/' + suite + '/test-' + test_name + '.c',
      include_directories : test_includes,
      link_with : lib,
      install : false,
    )
    test(suite + '-' + test_name, test_exe, timeout: 10)
  endforeach
endforeach
