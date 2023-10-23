#!/usr/bin/env bash

set -e

cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=ON
cmake --build build
