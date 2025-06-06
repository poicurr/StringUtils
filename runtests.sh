#!/bin/bash
set -e

BUILD_DIR="build"

cmake -B $BUILD_DIR -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build $BUILD_DIR

$BUILD_DIR/tests
