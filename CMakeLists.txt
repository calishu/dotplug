cmake_minimum_required(VERSION 3.24)
project(dotplug LANGUAGES CXX)
cmake_policy(SET CMP0135 NEW)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# CLI11
find_package(CLI11 QUIET)
if(CLI11_FOUND)
	message(CHECK_START "Found CLI11!")
else()
	message(CHECK_START "Fetching CLI11 source...")
	FetchContent_Declare(CLI11 URL https://github.com/CLIUtils/CLI11/archive/v2.5.0.zip)
endif()
find_package(CLI11 REQUIRED)

# tomlplusplus
find_package(tomlplusplus QUIET)
if(tomlplusplus_FOUND)
	message(CHECK_START "Found tomlplusplus!")
else()
	message(CHECK_START "Fetching tomlplusplus source...")
	FetchContent_Declare(tomlplusplus URL https://github.com/marzer/tomlplusplus/archive/v3.4.0.zip)
endif()
find_package(tomlplusplus REQUIRED)

# libgit2
find_package(libgit2 QUIET)
if(libgit2_FOUND)
	message(CHECK_START "Found libgit2!")
else()
	message(CHECK_START "Fetching libgit2 source...")
	FetchContent_Declare(libgit2 URL https://github.com/libgit2/libgit2/archive/v1.9.1.zip)
endif()
find_package(libgit2 REQUIRED)

link_libraries(CLI11::CLI11 tomlplusplus::tomlplusplus libgit2::libgit2package)
include_directories(include)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(
		CONFIG_PATH="/.config/dotplug/"
		DOTFILES_PATH="/.local/share/dotplug/dotfiles/"
		DESTINATION_PATH="/.config/"
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(
		CONFIG_PATH="/.local/share/dotplug-dev/"
		DOTFILES_PATH="/.local/share/dotplug-dev/dotfiles/"
		DESTINATION_PATH="/.cache/dotplug-dev/"
	)
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(dotplug ${SOURCES})
