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

### Run benchmark tests with XML report output

Use [run-benchmarks.sh](./run-benchmarks.sh) or [run-benchmarks.bat](./run-benchmarks.bat) to execute the benchmark tests. These are shortcuts for the following command:

```shell
./build/test/ConvolutionBenchmarksTests [performance] --reporter XML::out=./build/benchmark/benchmark-report.xml --reporter console::out=-::colour-mode=ansi
```
The resulting XML contains all test results including benchmark values as the following snippet shows:
```xml
    <BenchmarkResults name="(JohT) kernelCentricConvolution" samples="100" resamples="100000" iterations="1" clockResolution="17.1192" estimatedDuration="6.7594e+06">
      <!-- All values in nano seconds -->
      <mean value="67934.4" lowerBound="67736.9" upperBound="68257.3" ci="0.95"/>
      <standardDeviation value="1267.56" lowerBound="870.005" upperBound="1755.71" ci="0.95"/>
      <outliers variance="0.113537" lowMild="0" lowSevere="1" highMild="2" highSevere="15"/>
    </BenchmarkResults>
```

## Dependencies

### Create and update package-lock.cmake
As described in [CPM Package-lock](https://github.com/cpm-cmake/CPM.cmake/wiki/Package-lock), `package-lock.cmake` can be created or updated using the following commands. The advantage of this is that there is one distinct file that contains all
dependency versions.

```shell
cmake -H. -Bbuild
cmake --build build --target cpm-update-package-lock 
```