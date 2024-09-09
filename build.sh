#!/usr/bin/bash

set -e

mkdir -p build
rsync -a static/ build/

cd build
cmake ..
make -j4

mv compile_commands.json ../

rm sokobox.zip || true
zip -r9 sokobox.zip sokobox sokobox.ini res

./sokobox
cd ..
