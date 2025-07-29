project('dotplug', 'cpp', default_options: ['cpp_std=c++23'])

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
nlohmann_json = dependency('nlohmann_json', method: 'pkg-config', required: true)

src_files = files(
  'src/context.cpp',
  'src/main.cpp',
  'src/modules/apply.cpp',
  'src/modules/install.cpp',
  'src/modules/list.cpp',
  'src/modules/new_config.cpp',
  'src/modules/remove.cpp',
  'src/modules/validator.cpp',
  'src/utils/config.cpp',
  'src/utils/format_check.cpp',
  'src/utils/logging.cpp',
  'src/utils/symlinks.cpp',
)
include_dir = include_directories('include/')

if get_option('RELEASE')
  add_project_arguments(
    '-DDOTFILES_PATH="/.local/share/dotplug/dotfiles/"',
    '-DSHARE_PATH="/.local/share/dotplug/"',
    '-DDESTINATION_PATH="/.config/"',
    '-DCONFIG_PATH="/.config/dotplug/"',
    '-DSTATE_PATH="/.local/state/dotplug/"',
    language: 'cpp',
  )
else
  add_project_arguments(
    '-DDOTFILES_PATH="/.local/share/dotplug-dev/dotfiles/"',
    '-DDESTINATION_PATH="/.cache/dotplug-dev/"',
    '-DCONFIG_PATH="/.local/share/dotplug-dev/"',
    '-DSTATE_PATH="/.local/share/dotplug-dev/"',
    '-DSHARE_PATH="/.local/share/dotplug-dev/"',
    language: 'cpp',
  )
endif

executable(
  'dotplug',
  src_files,
  dependencies: [cli11_dep, tomlpp_dep, nlohmann_json],
  include_directories: include_dir,
)
