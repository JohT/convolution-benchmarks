#!/bin/sh -x

# Run this script from the root directory of this repository

# Create the "benchmark" directory inside the "build" directory if it doesn't exist yet
mkdir -p ./build/benchmark

# Run the benchmark tests and write the results as a XML to the "benchmark" directory and the console
./build/test/ConvolutionBenchmarksTests [performance] --reporter XML::out=./build/benchmark/benchmark-report.xml --reporter console::out=-::colour-mode=ansi