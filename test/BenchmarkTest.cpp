#include "../source/FIRFilter.h"
#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <sys/stat.h>

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename SampleType>
void convolution_full(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> out)
{
    auto const inSize = in.size();
    auto const kernelSize = kernel.size();
    auto const outSize = inSize + kernelSize - 1;
    for (auto i(0); i < outSize; ++i)
    {
        auto const jmn = (i >= kernelSize - 1) ? i - (kernelSize - 1) : 0;
        auto const jmx = (i < inSize - 1) ? i : inSize - 1;
        for (auto j(jmn); j <= jmx; ++j)
        {
            out[i] += (in[j] * kernel[i - j]);
        }
    }
}

template<typename SampleType>
void convolution_full_InnerLoopVectorization(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> out)
{
    auto const inSize = in.size();
    auto const kernelSize = kernel.size();
    auto const outSize = inSize + kernelSize - 1;
    for (auto i(0); i < outSize; ++i)
    {
        auto const jmn = (i >= kernelSize - 1) ? i - (kernelSize - 1) : 0;
        auto const jmx = (i < inSize - 1) ? i : inSize - 1;
        for (auto j(jmn); j <= jmx; j += 4)
        {
            out[i] += (in[j] * kernel[i - j]) +
                      (in[j + 1] * kernel[i - j + 1]) +
                      (in[j + 2] * kernel[i - j + 2]) +
                      (in[j + 3] * kernel[i - j + 3]);
        }
    }
}

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename SampleType>
void convolution_valid(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> out)
{
    int const inSize = in.size();
    int const kernelSize = kernel.size();
    std::vector<SampleType> const &min_v = (inSize < kernelSize) ? in : kernel;
    std::vector<SampleType> const &max_v = (inSize < kernelSize) ? kernel : in;
    int const outSize = std::max(inSize, kernelSize) - std::min(inSize, kernelSize) + 1;
    for (auto i(0); i < outSize; ++i)
    {
        for (int j(min_v.size() - 1), k(i); j >= 0; --j)
        {
            out[i] += min_v[j] * max_v[k];
            ++k;
        }
    }
}

// generate a random vector of size n with values in [-1, 1] with std::random
template<typename T>
std::vector<T> generateVectorWithRealNumbers(int n, T min, T max)
{
    std::vector<T> vectorWithRandomNumbers(n, T());
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<> distribution(min, max);
    for (T &value : vectorWithRandomNumbers)
    {
        value = distribution(gen);
    }
    return vectorWithRandomNumbers;
}

TEST_CASE("Convolution with/without SIMD Benchmarks", "[performance]")
{
    auto input = generateVectorWithRealNumbers(16384, -1.0F, 1.0F);
    auto kernel = generateVectorWithRealNumbers(16, 0.0F, 1.0F);

    BENCHMARK_ADVANCED("convolution_full")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        meter.measure([input, kernel, out]
                      {
                          convolution_full(input, kernel, out);
                          return out; });
    };

    BENCHMARK_ADVANCED("convolution_full_InnerLoopVectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        meter.measure([input, kernel, out]
                      {
                          convolution_full_InnerLoopVectorization(input, kernel, out);
                          return out; });
    };

    BENCHMARK_ADVANCED("convolution_valid")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto inSize = input.size();
        const auto kernelSize = kernel.size();
        const auto outSize = std::max(inSize, kernelSize) - std::min(inSize, kernelSize) + 1;
        std::vector<float> out(outSize);

        meter.measure([input, kernel, out]
                      {
                          convolution_valid(input, kernel, out);
                          return out;
                      });
    };

    BENCHMARK_ADVANCED("applyFirFilterSingle")
    (Catch::Benchmark::Chronometer meter)
    {
        fir::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return fir::applyFirFilterSingle(inputAligned); });
    };

    BENCHMARK_ADVANCED("applyFirFilterInnerLoopVectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        fir::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return fir::applyFirFilterInnerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("applyFirFilterOuterLoopVectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        fir::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return fir::applyFirFilterOuterLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("applyFirFilterOuterInnerLoopVectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        fir::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return fir::applyFirFilterOuterInnerLoopVectorization(inputAligned); });
    };
}