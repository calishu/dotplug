#!/bin/sh
find src include -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i