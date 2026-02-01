project('dotplug', 'cpp', default_options: ['cpp_std=c++23'])

cmake = import('cmake')
picosha2_sub = cmake.subproject('PicoSHA2')

cli11_dep = dependency('CLI11', required: true)
nlohmann_dep = dependency('nlohmann_json', required: true)
picosha2_dep = picosha2_sub.dependency('picosha2')

src_files = files(
  'src/main.cpp',
  'src/cli.cpp',
  'src/logging.cpp',
  'src/commands/version.cpp',
)
include_dir = include_directories('include/')

executable(
  'dotplug',
  src_files,
  dependencies: [cli11_dep, nlohmann_dep, picosha2_dep],
  include_directories: include_dir,
)
