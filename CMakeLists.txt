cmake_minimum_required(VERSION 4.0.3)
project(dotplug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dotplug
  src/main.cpp
  src/modules/list.cpp
  src/modules/validator.cpp
  src/utils/config.cpp
)

add_compile_definitions(
  DOTFILES_PATH="/Dokumente/Projekte/dotplug/test_config/"
)

target_include_directories(dotplug PRIVATE src/headers external)
