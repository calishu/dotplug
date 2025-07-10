cmake_minimum_required(VERSION 4.0.3)
project(dotplug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dotplug
  src/main.cpp
  src/globals.cpp
  src/modules/list.cpp
  src/modules/remove.cpp
  src/modules/validator.cpp
  src/modules/new_config.cpp
  src/utils/config.cpp
)

add_compile_definitions(
  DOTFILES_PATH="/Dokumente/Projekte/dotplug/test_config/"
  DESTINATION_PATH="/Dokumente/Projekte/dotplug/target/"
)

target_include_directories(dotplug PRIVATE src/headers external)
