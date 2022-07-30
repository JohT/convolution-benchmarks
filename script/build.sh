#!/bin/sh -x

# Run this script from the root directory of this repository

# Configure the build with CMake (this may vary depending on the system)
# -B: Use "build" as the build directory
# -G: Use Ninja as generator
# -S: Use the current directory as the source directory
cmake -B build -G Ninja -S . \
 --no-warn-unused-cli \
 -DCMAKE_BUILD_TYPE:STRING=Release \
 -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
 -DENFORCE_VECTOR_EXTENSION_SSE2:BOOL=FALSE \
 -DENFORCE_VECTOR_EXTENSION_AVX:BOOL=FALSE \
 -DENFORCE_VECTOR_EXTENSION_AVX2:BOOL=FALSE

# Start the build with CMake
cmake --build build --config Release --target all --