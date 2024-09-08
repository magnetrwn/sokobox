#!/usr/bin/bash

set -e

mkdir -p build bin
rsync -a static/ build/

cd build
cmake ..
make -j4

mv compile_commands.json ../
