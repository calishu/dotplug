project('dotplug', 'cpp', default_options: ['cpp_std=c++20'])

run_command(
  'conan',
  'install',
  '.',
  '--output-folder=build',
  '--build=missing',
  check: true,
)

cli11_dep = dependency('CLI11', method: 'pkg-config', required: true)
tomlpp_dep = dependency('tomlplusplus', method: 'pkg-config', required: true)

src_files = files(
  'src/context.cpp',
  'src/main.cpp',
  'src/modules/list.cpp',
  'src/modules/new_config.cpp',
  'src/modules/remove.cpp',
  'src/modules/validator.cpp',
  'src/utils/config.cpp',
)
include_dir = include_directories('src/headers')

add_project_arguments(
  '-DDOTFILES_PATH="/Dokumente/Projekte/dotplug/test_config/"',
  '-DDESTINATION_PATH="/Dokumente/Projekte/dotplug/target/"',
  language: 'cpp',
)

executable(
  'dotplug',
  src_files,
  dependencies: [cli11_dep, tomlpp_dep],
  include_directories: include_dir,
)
