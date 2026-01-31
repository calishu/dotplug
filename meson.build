project('dotplug', 'cpp', default_options: ['cpp_std=c++23'])

cli11_dep = dependency('CLI11', required: true)
nlohmann_dep = dependency('nlohmann_json', required: true)

src_files = files(
  'src/main.cpp',
  'src/logging.cpp'
)
include_dir = include_directories('include/')

executable(
  'dotplug',
  src_files,
  dependencies: [cli11_dep, nlohmann_dep],
  include_directories: include_dir,
)
