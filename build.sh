#!/usr/bin/bash

set -e

mkdir -p build
rsync -a static/ build/

cd lib/raylib
mkdir -p build && cd build
cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DGRAPHICS=GRAPHICS_API_OPENGL_43
cmake --build . -j$(nproc) --config Release
cd ../../..

cd build
cmake -G "Unix Makefiles" ..
make -j$(nproc)

mv compile_commands.json ../

rm sokobox.zip || true
# zip -r9 sokobox.zip sokobox sokobox.ini res

./sokobox
cd ..
