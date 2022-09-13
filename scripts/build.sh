#!/usr/bin/env bash

set -e

cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
