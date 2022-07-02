#!/usr/bin/env bash

readarray -d '' SOURCES < <(find test \( -name '*.c' -o -name '*.h' \) -type f -print0)

clang-format -i "${SOURCES[@]}"
