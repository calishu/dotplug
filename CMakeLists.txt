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
	message("Found CLI11 (package)")
else()
	message("Could not find CLI11, fetching source...")
	FetchContent_Declare(CLI11 URL https://github.com/CLIUtils/CLI11/archive/v2.5.0.zip)
endif()
find_package(CLI11 REQUIRED)
link_libraries(CLI11::CLI11)

# tomlplusplus
find_package(tomlplusplus QUIET)
if(tomlplusplus_FOUND)
	message("Found tomlplusplus (package)")
else()
	message("Could not find tomlplusplus, fetching source...")
	FetchContent_Declare(tomlplusplus URL https://github.com/marzer/tomlplusplus/archive/v3.4.0.zip)
endif()
find_package(tomlplusplus REQUIRED)
link_libraries(tomlplusplus::tomlplusplus)

include_directories(include)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(
		CONFIG_PATH="/.config/dotplug/"
		DOTFILES_PATH="/.local/share/dotplug/dotfiles/"
		DESTINATION_PATH="/.config/"
		STATE_PATH="/.local/state/dotplug"
	)
else() # use dev paths for anything other than release
	add_compile_definitions(
		CONFIG_PATH="/.local/share/dotplug-dev/"
		DOTFILES_PATH="/.local/share/dotplug-dev/dotfiles/"
		DESTINATION_PATH="/.cache/dotplug-dev/"
		STATE_PATH="/.local/share/dotplug-dev/"
	)
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(dotplug ${SOURCES})
