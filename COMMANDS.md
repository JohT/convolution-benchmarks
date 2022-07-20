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
./../build/test/GettingStartedWithSIMDTests --break --success --skip-benchmarks
```