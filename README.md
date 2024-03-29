# Benchmark Convolution Implementations

This repository takes different C++ implementations of the [convolution](https://en.wikipedia.org/wiki/Convolution) algorithm and provides fully automated benchmarks visualized as charts. It might also be used as a starting point or template for other C++ benchmark projects.

## 🚀 Features

- Insights on how a classical convolution algorithm can be implemented and optimized: [BenchmarkCharts.md](./chart/BenchmarkCharts.md)
- A simple (6 lines of code), easy to read (no intrinsics), yet fast convolution implementation: [inputPerKernelValueTransposed](./source/JohTConvolution.h)
- Fully automated (CLI) visualization of [Catch2](https://github.com/catchorg/Catch2) benchmark results using [vega-lite](https://vega.github.io/vega-lite) charts: [chart/README.md](./chart/README.md)
- [GitHub Actions](https://docs.github.com/en/actions) workflow for fully automated benchmarks on Linux, MacOS and Windows: [continuous-integration.yml](.github/workflows/continuous-integration.yml)
- [Renovate](https://github.com/renovatebot/renovate) configuration to update [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) managed C++ dependencies: [renovate.json](./renovate.json)

## 📖 Related Blog Articles

- [A different approach to Convolution](https://joht.github.io/johtizen/algorithm/2022/10/22/a-different-approach-to-convolution.html)
- [Visualize Catch2 benchmarks with Vega-Lite](https://joht.github.io/johtizen/data/2022/09/05/visualize-catch2-benchmarks-with-vega-lite.html)
- [Keep your C++ dependencies up-to-date with Renovate & CPM](https://joht.github.io/johtizen/automation/2022/08/03/keep-your-cpp-dependencies-up-to-date.html)

## 📈 Results

[BenchmarkCharts.md](./chart/BenchmarkCharts.md) contains the benchmark results visualized as bar charts.

## ⚒️ Tools

- Needs [CMake](https://cmake.org/download) (e.g. with the [Visual Studio Installer](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)) to build the project.
- Needs [node-canvas](https://github.com/Automattic/node-canvas#compiling) dependencies to create SVG vector graphics files.
- Needs [nodejs](https://nodejs.org) to build the JavaScript based charts.
- Recommends [Ninja](https://ninja-build.org/) as "a small build system with a focus on speed".
- Uses [cpm](https://github.com/cpm-cmake/CPM.cmake) as a "Setup-free CMake dependency management" for C++.
- Uses [Catch2](https://github.com/catchorg/Catch2) as Unit-Test and Benchmark-Framework for C++.
- Uses [vega-lite](https://vega.github.io/vega-lite) to visualize the benchmark results as a bar chart.
- Uses [GitHub Actions](https://docs.github.com/en/actions) to fully automate build, benchmarks and visualization.
- Uses [Renovate](https://github.com/renovatebot/renovate) to update the dependencies automatically.

## ⚡️ Commands

- Run [script/run-all.sh](./script/run-all.sh) or [script\run-all.bat](./script/run-all.bat) to build and test the project, run the benchmarks and create the charts.
- Run [script/benchmark-with-charts.sh](./script/benchmark-with-charts.sh) or [script\benchmark-with-charts.bat](./script/benchmark-with-charts.bat) to run the benchmarks and generate the charts without rebuilding the project.
- Further commands and a detailed description can be found in [COMMANDS.md](./COMMANDS.md).
- [chart/COMMANDS.md](./chart/COMMANDS.md) describes the commands to create the charts.

## ⚙️ Compiler Options for vectorization reports

The following compiler options are used to get detailed vectorization reports. They are defined in [CompilerOptions.cmake](./cmake/CompilerOptions.cmake).

### CLang

These compile options are used as described in [Auto-Vectorization in LLVM](https://llvm.org/docs/Vectorizers.html) and [Clang command line argument reference](https://releases.llvm.org/9.0.0/tools/clang/docs/ClangCommandLineReference.html):

- `-Rpass=loop-vectorize` identifies loops that were successfully vectorized.
- `-Rpass-missed=loop-vectorize` identifies loops that failed vectorization and indicates if vectorization was specified.
- `-Rpass-analysis=loop-vectorize` identifies the statements that caused vectorization to fail. If in addition -fsave-optimization-record is provided, multiple causes of vectorization failure may be listed (this behavior might change in the future).
- `-fsave-optimization-record` generate a YAML optimization record file.

### GCC

These compile options are used with GCC as described in [GCC Developer Options](https://gcc.gnu.org/onlinedocs/gcc-10.1.0/gcc/Developer-Options.html):

- `-fopt-info-vec-optimized` prints information when an optimization from vectorization passes is successfully.
- `-fopt-info-vec-missed` prints information about missed optimization opportunities from vectorization passes.

### MSVC

These compile options are used with MSVC as described in [Auto-Vectorizer Reporting Level](https://docs.microsoft.com/en-us/cpp/build/reference/qvec-report-auto-vectorizer-reporting-level?view=msvc-170) and [Auto-Parallelization and Auto-Vectorization](https://docs.microsoft.com/en-us/cpp/parallel/auto-parallelization-and-auto-vectorization?view=msvc-170):

- `/Qvec-report:2` outputs an informational message for loops that are vectorized and for loops that are not vectorized, together with a reason code.

- `/arch:AVX` specifies the architecture for code generation (here AVX).

## 🔭 Online Tools

### Godbolt Compiler Explorer

[Godbolt Compiler Explorer](https://gcc.godbolt.org) is great tool to analyze small blocks of code and get insights in what the compiler does. Its ideal to try things out. The following two links show an easy yet fast convolution implementation, that works well with compiler optimization and vectorization.  

- [Convolution Implementation "inputPerKernelValueTransposed" with CLang 13.0.0](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:4,positionColumn:1,positionLineNumber:4,selectionStartColumn:1,selectionStartLineNumber:4,startColumn:1,startLineNumber:4),source:'%23include+%3Cvector%3E%0A%0Avoid+inputPerKernelValueTransposed(const+float*+const+input,+const+int+inputLength,+const+float*+const+kernel,+const+int+kernelLength,+float*+const+output)+%7B%0A++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++//+Make+it+obvious+for+the+compiler+(especially+MSVC)+that+the+factor+is+constant.%0A++++++++const+auto+kernelValue+%3D+kernel%5BkernelIndex%5D%3B%0A%0A++++++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++++++//+It+seems+to+be+beneficial+to+put+the+constant+factor+last+when+MSVC+compile+option+%22/fp:fast%22+is+activated.%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernelValue%3B%0A++++++++%7D%0A++++%7D%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang1300,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-fPIC+-O3+-ffast-math+-Rpass%3Dloop-vectorize+-Rpass-missed%3Dloop-vectorize+-Rpass-analysis%3Dloop-vectorize',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+13.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+13.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)
- [Convolution Implementation "inputPerKernelValueTransposed" with CLang 10.0.0 and AVX](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:4,positionColumn:1,positionLineNumber:4,selectionStartColumn:1,selectionStartLineNumber:4,startColumn:1,startLineNumber:4),source:'%23include+%3Cvector%3E%0A%0Avoid+inputPerKernelValueTransposed(const+float*+const+input,+const+int+inputLength,+const+float*+const+kernel,+const+int+kernelLength,+float*+const+output)+%7B%0A++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++//+Make+it+obvious+for+the+compiler+(especially+MSVC)+that+the+factor+is+constant.%0A++++++++const+auto+kernelValue+%3D+kernel%5BkernelIndex%5D%3B%0A%0A++++++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++++++//+It+seems+to+be+beneficial+to+put+the+constant+factor+last+when+MSVC+compile+option+%22/fp:fast%22+is+activated.%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernelValue%3B%0A++++++++%7D%0A++++%7D%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang1000,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B11+-fPIC+-O3+-ffast-math+-Rpass%3Dloop-vectorize+-Rpass-missed%3Dloop-vectorize+-Rpass-analysis%3Dloop-vectorize+-mavx',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+10.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+10.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)
- [Convolution Implementation "inputPerKernelValueTransposed" with MSVC](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:2,endLineNumber:13,positionColumn:2,positionLineNumber:13,selectionStartColumn:2,selectionStartLineNumber:13,startColumn:2,startLineNumber:13),source:'%23include+%3Cvector%3E%0A%0Avoid+inputPerKernelValueTransposed(const+float*+const+input,+const+int+inputLength,+const+float*+const+kernel,+const+int+kernelLength,+float*+const+output)+%7B%0A++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++//+Make+it+obvious+for+the+compiler+(especially+MSVC)+that+the+factor+is+constant.%0A++++++++const+auto+kernelValue+%3D+kernel%5BkernelIndex%5D%3B%0A%0A++++++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++++++//+It+seems+to+be+beneficial+to+put+the+constant+factor+last+when+MSVC+compile+option+%22/fp:fast%22+is+activated.%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernelValue%3B%0A++++++++%7D%0A++++%7D%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:vcpp_v19_32_x64,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'/std:c%2B%2B11+/O2+/EHsc+/Qvec-report:2+/arch:AVX2+/fp:fast',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x64+msvc+v19.32+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x64+msvc+v19.32+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

### Quick C++ Benchmark

[Quick C++ Benchmark](https://quick-bench.com/) is another great tool that compares the performance of two code blocks with each other.

Copy & Paste the following code snipped into [Quick C++ Benchmark](https://quick-bench.com/). It shows the difference between the kernel and the input values as outer loop with a "direct form transposed" convolution implementation.

<details>
  <summary>Code Snipped to compare the iteration over the kernel values in the inner vs. the outer loop</summary>
  
```c++
#include <vector>
#include <span>
#include <random>

template<typename T>
std::vector<T> randomNumbers(int n, T min, T max)
{
    std::vector<T> vectorWithRandomNumbers(n, T());
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<T> distribution(min, max);
    for (T &value : vectorWithRandomNumbers)
    {
        value = distribution(gen);
    }
    return vectorWithRandomNumbers;
}

template<typename ValueType>
void convolutionKernelInner(const std::span<const ValueType> &input, const std::span<const ValueType> &kernel, const std::span<ValueType> &output)
{
    const int inputLength = input.size();
    const int kernelLength = kernel.size();
    
    for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
    {
      const auto inputValue = input[inputIndex];
      for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            output[inputIndex + kernelIndex] += inputValue * kernel[kernelIndex];
        }
    }
}

template<typename ValueType>
void convolutionKernelOuter(const std::span<const ValueType> &input, const std::span<const ValueType> &kernel, const std::span<ValueType> &output)
{
    const auto inputLength = input.size();
    const auto kernelLength = kernel.size();
    
    for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
    {
        const auto kernelValue = kernel[kernelIndex];
        for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
        {
            output[inputIndex + kernelIndex] += input[inputIndex] * kernelValue;
        }
    }
}

static void kernelInner(benchmark::State& state) {
  // Code before the loop is not measured
  const auto & input = randomNumbers(16384, -1.0F, 1.0F);
  const auto & kernel = randomNumbers(16, 0.0F, 1.0F);

  const auto convolutionLength = input.size() + kernel.size() - 1;
  auto output = std::vector<float>(convolutionLength, 0.0F);
    
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    convolutionKernelInner(std::span(input), std::span(kernel), std::span(output));
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(output);
  }
}
// Register the function as a benchmark
BENCHMARK(kernelInner);

static void kernelOuter(benchmark::State& state) {
  // Code before the loop is not measured
  const auto & input = randomNumbers(16384, -1.0F, 1.0F);
  const auto & kernel = randomNumbers(16, 0.0F, 1.0F);

  const auto convolutionLength = input.size() + kernel.size() - 1;
  auto output = std::vector<float>(convolutionLength, 0.0F);
    
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    convolutionKernelOuter(std::span(input), std::span(kernel), std::span(output));
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(output);
  }
}
// Register the function as a benchmark
BENCHMARK(kernelOuter);
```

</details>

## 📜 History

This repository was initially intended to explore Single Instruction Multiple Data ([SIMD](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data)) in C++. Since convolution is such an essential part of filtering in digital signal processing and a central part of convolutional neuronal networks ([CNN](https://en.wikipedia.org/wiki/Convolutional_neural_network)), it seemed obvious to try that first.

A [complex](https://en.wikipedia.org/wiki/Cynefin_framework#Complex) topic like this benefits from a "experiment - evaluate" approach to get started. [Catch2](https://github.com/catchorg/Catch2) is used to assure, that the implementations are correct. It is also used to benchmark their performance. [Compiler Options for vectorization reports](#⚙️-compiler-options-for-vectorization-reports) get insights into what the compiler does. [Online-Tools](#🔭-online-tools) are used to exploratory experiment with implementations. Finally, to get a visual response, [vega-lite](https://vega.github.io/vega-lite) is used to create bar charts of the benchmark results.

## 🔎 References

- [Efficient FIR Filter Implementation with SIMD (Jan Wilczek)](https://thewolfsound.com/fir-filter-with-simd)
- [Vectorization part1. Intro. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/24/Vectorization_part1)
- [Microsoft® Visual Studio Compiler - Vectorizer and parallelizer messages](https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/vectorizer-and-parallelizer-messages?view=msvc-170)
- [Intel® Programming Guidelines for Vectorization](https://www.intel.com/content/www/us/en/develop/documentation/cpp-compiler-developer-guide-and-reference/top/optimization-and-programming/vectorization/automatic-vectorization/programming-guidelines-for-vectorization.html)
- [FIR Structures](https://www.ni.com/docs/de-DE/bundle/labview-2014-digital-filter-design-toolkit-api-ref/page/lvdfdtconcepts/fir_filter_specs.html)
- [Permission denied for build.sh file (StackTrace)](https://stackoverflow.com/questions/42154912/permission-denied-for-build-sh-file)
- [FIRBenchmarks (GitHub)](https://github.com/jatinchowdhury18/FIRBenchmarks)
