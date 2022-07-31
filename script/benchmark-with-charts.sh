#!/bin/sh -x

# Run this script from the root directory of this repository

# Run the benchmark tests and create the charts
./script/run-benchmarks.sh
./script/create-charts.sh