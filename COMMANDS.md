# Commands
This document lists the steps and commands that were executed to set up the project.
It also contains the commands that are used to build it.

## Build

### Prepare a "Debug" build with Ninja as generator
```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B build -G Ninja
```

### Start the "Debug" build for all targets
```shell
cmake --build ./build --config Debug 
```

### Prepare a "Release" build with Ninja as generator
```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -S . -B ./build -G Ninja
```

### Start the "Release" build for all targets
```shell
cmake --build ./build --config Release 
```

## Tests

### Start Unit-Tests with CTest
```shell
ctest --test-dir build/test
```

### Run Catch2 Unit-Tests directly

```shell
./../build/test/ConvolutionBenchmarksTests --break --success --skip-benchmarks
```

## Dependencies

### Create and update package-lock.cmake
As described in [CPM Package-lock](https://github.com/cpm-cmake/CPM.cmake/wiki/Package-lock), `package-lock.cmake` can be created or updated using the following commands. The advantage of this is that there is one distinct file that contains all
dependency versions.

```shell
cmake -H. -Bbuild
cmake --build build --target cpm-update-package-lock 
```