# Benchmark Charts

## MacOS on Intel with AVX compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on Intel](./AppleClang-MacOSX-x86_64AVX/BenchmarkChart.svg)

## MacOS on M1 with Neon compiled with [CLang](https://clang.llvm.org) 
![bar chart showing the results of the benchmarks for MacOs on M1](./AppleClang-macOS-arm64/BenchmarkChart.svg)

## Windows on Intel with SSE (no AVX) compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with SSE](./MSVC-Windows-AMD64SSE2/BenchmarkChart.svg)

## Windows on Intel with AVX compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with AVX](./MSVC-Windows-AMD64AVX/BenchmarkChart.svg)

## Windows on Intel with AVX2 compiled with [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus)
![bar chart showing the results of the benchmarks for Windows on Intel with AVX2](./MSVC-Windows-AMD64AVX2/BenchmarkChart.svg)

## Floating Point compile options

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:precise` (default)
![bar chart showing the results of the benchmarks with fast floating point compile option](./other/BenchmarkChartWindowsDefaultFloatingPoint.png)

### [Microsoft Visual C++ (MSVC)](https://visualstudio.microsoft.com/de/vs/features/cplusplus) with `/fp:fast`
![bar chart showing the results of the benchmarks with fast floating point compile option](./other/BenchmarkChartWindowsFastFloatingPoint.png)