# Benchmark Convolution Implementations

This repository is intended to accompany the first steps in C++ with Single Instruction Multiple Data (SIMD).

## Prerequisites

- Installed [Git](https://git-scm.com/downloads)
- Installed [CMake](https://cmake.org/download/) (e.g. with the [Visual Studio Installer](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170))
- Installed [Ninja](https://ninja-build.org/), a small build system with a focus on speed.

## Commands

Commands to build and test the project can be found in [COMMANDS.md](./COMMANDS.md).
[chart/COMMANDS.md](./chart/COMMANDS.md) describes the commands to run the benchmark tests and create the charts.

Nontheless, here are the essential commands to build the project, run the benchmark tests and create the charts:

```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -S . -B ./build -G Ninja
cmake --build ./build --config Release
cd charts
npm install
npm run create-benchmark-charts
```

## Online Tools

### Godbolt Compiler Explorer

[Godbolt Compiler Explorer](https://gcc.godbolt.org) is great tool to analyze small blocks of code and get insights in what the compiler does. Its ideal to try things out. The following two links show an easy yet fast convolution implementation, that works well with comiler optimization and vectorization.  

- [Convolution Implementation "inputPerKernelValueTransposed" with CLang](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:8,positionColumn:1,positionLineNumber:8,selectionStartColumn:1,selectionStartLineNumber:8,startColumn:1,startLineNumber:8),source:'%23include+%3Cvector%3E%0A%23include+%3Cspan%3E%0A%0Avoid+inputPerKernelValueTransposed(const+std::span%3Cconst+float%3E+%26input,+const+std::span%3Cconst+float%3E+%26kernel,+const+std::span%3Cfloat%3E+%26output)+%7B%0A++++//+Make+it+obvious+for+the+compiler+(e.g.+MSVC)+that+the+size+of+the+arrays+are+constant.%0A++++const+auto+inputLength+%3D+static_cast%3Cint%3E(input.size())%3B%0A++++const+auto+kernelLength+%3D+static_cast%3Cint%3E(kernel.size())%3B%0A%0A++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++//+Make+it+obvious+for+the+compiler+(especially+MSVC)+that+the+factor+is+constant.%0A++++++++const+auto+kernelValue+%3D+kernel%5BkernelIndex%5D%3B%0A%0A++++++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++++++//+It+seems+to+be+beneficial+to+put+the+constant+factor+last+when+MSVC+compile+option+%22/fp:fast%22+is+activated.%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernelValue%3B%0A++++++++%7D%0A++++%7D%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang1301,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'--std%3Dc%2B%2B20+-fPIC+-O3+-ffast-math+-Rpass%3Dloop-vectorize+-Rpass-missed%3Dloop-vectorize+-Rpass-analysis%3Dloop-vectorize',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+13.0.1+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+13.0.1+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)
- [Convolution Implementation "inputPerKernelValueTransposed" with MSVC](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:74,endLineNumber:14,positionColumn:74,positionLineNumber:14,selectionStartColumn:74,selectionStartLineNumber:14,startColumn:74,startLineNumber:14),source:'%23include+%3Cvector%3E%0A%23include+%3Cspan%3E%0A%0Avoid+inputPerKernelValueTransposed(const+std::span%3Cconst+float%3E+%26input,+const+std::span%3Cconst+float%3E+%26kernel,+const+std::span%3Cfloat%3E+%26output)+%7B%0A++++//+Make+it+obvious+for+the+compiler+(e.g.+MSVC)+that+the+size+of+the+arrays+are+constant.%0A++++const+auto+inputLength+%3D+static_cast%3Cint%3E(input.size())%3B%0A++++const+auto+kernelLength+%3D+static_cast%3Cint%3E(kernel.size())%3B%0A%0A++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++//+Make+it+obvious+for+the+compiler+(especially+MSVC)+that+the+factor+is+constant.%0A++++++++const+auto+kernelValue+%3D+kernel%5BkernelIndex%5D%3B%0A%0A++++++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++++++//+It+seems+to+be+beneficial+to+put+the+constant+factor+last+when+MSVC+compile+option+%22/fp:fast%22+is+activated.%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernelValue%3B%0A++++++++%7D%0A++++%7D%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:vcpp_v19_32_x64,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'/std:c%2B%2B20+/MP+/O2+/EHsc+/Qvec-report:2+/arch:AVX2+/fp:fast',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x64+msvc+v19.32+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x64+msvc+v19.32+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

### Quick C++ Benchmark

[Quick C++ Benchmark](https://quick-bench.com/) is another great tool that compares the performance of two code blocks with each other.

Copy & Paste the following code snipped into [Quick C++ Benchmark](https://quick-bench.com/). It shows the difference between the kernel and the input values as outer loop with a "direct form transposed" convolution implementation.

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

## References

- [Vectorization part1. Intro. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/24/Vectorization_part1)
- [Vectorization part2. Warmup. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/27/Vectorization_warmup)
- [Vectorization part3. Compiler report. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/30/Compiler-optimization-report)
- [Vectorization part4. Vectorization Width. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/02/Vectorization_width)
- [Vectorization part5. Multiversioning by data dependency. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/03/Multiversioning_by_DD)
- [Vectorization part6. Multiversioning by trip counts. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/09/Multiversioning_by_trip_counts)
- [Vectorization part7. Tips for writing vectorizable code. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/10/Tips_for_writing_vectorizable_code)
- [Microsoft Visual Studio Compiler - Vectorizer and parallelizer messages](https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/vectorizer-and-parallelizer-messages?view=msvc-170)
- [FIR Structures](https://www.ni.com/docs/de-DE/bundle/labview-2014-digital-filter-design-toolkit-api-ref/page/lvdfdtconcepts/fir_filter_specs.html)