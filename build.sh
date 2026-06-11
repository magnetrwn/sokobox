#!/usr/bin/bash

set -e

mkdir -p build
rsync -a static/ build/

cd build
cmake -G "Unix Makefiles" ..
make -j$(nproc)

mv compile_commands.json ../

rm sokobox.zip || true
# zip -r9 sokobox.zip sokobox sokobox.ini res

./sokobox
cd ..
