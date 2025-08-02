#!/bin/sh
set -e

required_version=19
current_version=$(clang-format --version | grep -o '[0-9]\+' | head -1)

if [ "$current_version" -lt "$required_version" ]; then
	echo "(error) clang-format version $required_version+ is required. You have clang-format version ${current_version}"
	exit 1
fi

find src include -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
