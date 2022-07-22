# Benchmark Charts

## MacOS on Intel compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on Intel](./BenchmarkChartMacOs.svg)

## MacOS on M1 with Neon compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on M1](./BenchmarkChart-AppleClang-macOS-arm64.svg)

## Windows on Intel with SSE (no AVX) compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with SSE](./BenchmarkChartWindowsSSE.svg)

## Windows on Intel with AVX2 compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with AVX2](./BenchmarkChartWindowsAVX2.svg)

## Floating Point compile options

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:precise` (default)
![bar chart showing the results of the benchmarks with fast floating point compile option](./BenchmarkChartWindowsDefaultFloatingPoint.png)

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:fast`
![bar chart showing the results of the benchmarks with fast floating point compile option](./BenchmarkChartWindowsFastFloatingPoint.png)