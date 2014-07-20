cmake -DCMAKE_TOOLCHAIN_FILE=~/Development/emsdk-portable/emscripten/1.21.0/cmake/Platform/Emscripten.cmake \
-DCMAKE_BUILD_TYPE=Debug \
-DEMSCRIPTEN=1 \
-G "Unix Makefiles" \
..
