# Commands
This document lists the steps and commands that were executed to set up the project.
It also contains the commands that are used to build it.

## Continuous Integration

[continuous-integration.yml](./.github/workflows/continuous-integration.yml) contains the [GitHub Actions](https://docs.github.com/en/actions) workflow configuration. It describes everything that is needed to setup, build and test the project, run the benchmarks and create the charts for Linux, Windows and MacOS and different CPU vector instruction set extensions. 

## Scripts

To simplify manual work, the following scripts are available. These need to be run from the root directory of the repository.

- Run [script/run-all.sh](./script/run-all.sh) or [script\run-all.bat](./script/run-all.bat) to run all the following commands in order.
- Run [script/benchmark-with-charts.sh](./script/benchmark-with-charts.sh) or [script\benchmark-with-charts.bat](./script/benchmark-with-charts.bat) to run the benchmark tests and generate charts without (re)building the project.
- Run [script/build.sh](./script/build.sh) or [script\build.bat](./script/build.bat) to compile and test the project.
- Run [script/run-benchmarks.sh](./script/run-benchmarks.sh) or [script\run-benchmarks.bat](./script/run-benchmarks.bat) to run the benchmark tests.
- Run [script/create-charts.sh](./script/create-charts.sh) or [script\create-charts.bat](./script/create-charts.bat) to create the benchmark charts.

## Dependencies

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) is a "Setup-free CMake dependency management" that is used to manage the dependencies of this project.

### Create and update package-lock.cmake
As described in [CPM Package-lock](https://github.com/cpm-cmake/CPM.cmake/wiki/Package-lock), `package-lock.cmake` can be created and updated using the following commands. 

```shell
cmake -H. -Bbuild
cmake --build build --target cpm-update-package-lock 
```

The advantage of this approach is that there is one distinct file that contains all dependencies and versions. [Renovate](https://github.com/renovatebot/renovate) is used to update those dependencies automatically as configured in [renovate.json](./renovate.json).

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

Use [run-benchmarks.sh](./script/run-benchmarks.sh) or [run-benchmarks.bat](./script/run-benchmarks.bat) to execute the benchmark tests. These are shortcuts for the following command:

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