project('tt', 'c', default_options: [])

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required : false)
cglm_dep = dependency('cglm', fallback : ['cglm', 'cglm_dep'])
glew_dep = dependency('glew', fallback : ['glew', 'glew_dep'])
glfw_dep = dependency('glfw3', fallback : ['glfw', 'glfw_dep'])
libpng_dep = dependency('libpng', fallback : ['libpng', 'libpng_dep'])

dependencies = [m_dep, glfw_dep, glew_dep, cglm_dep, libpng_dep]

includes = include_directories('include')


### Library ###

lib_sources = [
  'src/bt.c',
  'src/tt-behaviour-fail.c',
  'src/tt-behaviour-harvest-wood.c',
  'src/tt-behaviour-idle.c',
 # 'src/tt-behaviour-inventory-full.c',
  'src/tt-behaviour-loop.c',
#  'src/tt-behaviour-selector.c',
#  'src/tt-behaviour-selector-until.c',
#  'src/tt-behaviour-select-stockpile.c',
  'src/tt-behaviour-select-tree.c',
  'src/tt-behaviour-sequence.c',
  'src/tt-behaviour-sleep.c',
  'src/tt-behaviour-succeed.c',
  'src/tt-behaviour-walk-to-target.c',
  'src/tt-bitset.c',
  'src/tt-component-behaviour.c',
  'src/tt-component-behaviour-stack.c',
  'src/tt-component-brain.c',
  'src/tt-component-harvestable.c',
  'src/tt-component-position.c',
  'src/tt-component-sprite.c',
  'src/tt-component-target.c',
  'src/tt-component-wood.c',
  'src/tt-entities.c',
  'src/tt-error.c',
  'src/tt-renderer.c',
  'src/tt-resource-camera.c',
  'src/tt-resource-wood-map.c',
  'src/tt-spatial-index.c',
  'src/tt-storage-bitset.c',
  'src/tt-storage-vector.c',
  'src/tt-system-ai.c',
  'src/tt-system-behaviour.c',
  'src/tt-system-sprites.c',
  'src/tt-system-update-wood-map.c',
  'src/tt-texture.c',
  'src/tt-utils.c',
  'src/tt-vector.c',
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
  'tt-bitset': [
    'basic',
    'copy',
    'copy-empty',
  ],
  'tt-entities': [
    'create-callback',
    'delete-callback',
    'dont-reuse-before-maintain',
    'new',
    'release',
    'realloc-free-list',
    'iter',
    'iter-delete-first',
    'iter-delete-last',
    'iter-empty',
  ],
  'tt-behaviours': [
    'loop-sleep',
  ],
  'tt-storage-vector': [
  	'basic',
	'realloc',
	'removal',
  ]
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
