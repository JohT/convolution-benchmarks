#!/bin/sh -x

# Run this script from the root directory of this repository

# Runs all scripts in the right order to build the project, run the benchmarks and create the charts
./script/build.sh
./script/run-benchmarks.sh
./script/create-charts.sh