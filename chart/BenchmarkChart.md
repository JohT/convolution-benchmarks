# Benchmark Charts

## MacOS on Intel with SSE (no AVX) compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on Intel](./BenchmarkChart-AppleClang-MacOSX-x86_64AVX.svg)

## MacOS on M1 with Neon compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on M1](./BenchmarkChart-AppleClang-macOS-arm64.svg)

## Windows on Intel with SSE (no AVX) compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with SSE](./BenchmarkChart-MSVC-Windows-AMD64SSE2.svg)

## Windows on Intel with AVX compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with AVX](./BenchmarkChart-MSVC-Windows-AMD64AVX.svg)

## Windows on Intel with AVX2 compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with AVX2](./BenchmarkChart-MSVC-Windows-AMD64AVX2.svg)

## Floating Point compile options

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:precise` (default)
![bar chart showing the results of the benchmarks with fast floating point compile option](./BenchmarkChartWindowsDefaultFloatingPoint.png)

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:fast`
![bar chart showing the results of the benchmarks with fast floating point compile option](./BenchmarkChartWindowsFastFloatingPoint.png)