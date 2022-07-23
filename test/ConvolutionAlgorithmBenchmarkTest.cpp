#include "../source/Chaowang15Convolution.h"
#include "../source/JohTConvolution.h"
#include "../source/MatlabLikeConvolution.h"
#include "../source/WilczekConvolution.h"
#include "RandomVectorGenerator.h"
#include "TestVectors.h"


#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <cstdio>
#include <memory>
#include <string>

TEST_CASE("Convolution Implementation Benchmarks", "[.][performance]")
{
    const auto kernelLength = GENERATE(16, 1024);
    const auto &input = random_vector_generator::randomNumbers(16384, -1.0F, 1.0F);
    const auto &kernel = random_vector_generator::randomNumbers(kernelLength, 0.0F, 1.0F);

    const auto outputSize = input.size() + kernel.size() - 1;
    std::vector<float> output(outputSize);

    const std::span<const float> &inputSpan = std::span(input);
    const std::span<const float> &kernelSpan = std::span(kernel);
    const std::span<float> &outputSpan = std::span(output);

    // In some cases the input needs to be padded with (here one) zero sample(s) at the end.
    // This way the output length (= input length + filter length - 1)
    // becomes dividable by 4 so that the outer loop can be unrolled by 4.
    const auto dividableBy4PaddingLength = 4 - ((input.size() + kernel.size() - 1) % 4);
    auto dividableBy4Padding = std::vector<float>(dividableBy4PaddingLength, 0.0F);

    BENCHMARK_ADVANCED("kernelPerInputValue Transposed (kernel " + std::to_string(kernelLength) + ")")

    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelPerInputValueTransposed(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposed(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed InnerLoopUnrolled (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposedInnerLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed OuterLoopUnrolled (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposedOuterLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed InnerAndOuterLoopUnrolled (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed LoopFission (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposedLoopFission(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("inputPerKernelValue Transposed LoopFissionIndexArithmetic (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::inputPerKernelValueTransposedLoopFissionIndexArithmetic(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("kernelPerInputValue Transposed LoopFission (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelPerInputValueTransposedLoopFission(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(matlab-like) conv 'full' shape (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&input, &kernel, &output]
                      {
                          matlab_like::convolution_full(input, kernel, output);
                          return output; });
    };

    BENCHMARK_ADVANCED("(matlab-like) conv 'valid' shape pre padded (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        // "valid" means that the input needs to be padded with (kernel size - 1) zeroes
        // at the beginning and end of the data input vector before the convolution.
        auto paddedInput = std::vector<float>(input);
        auto padding = std::vector<float>(kernel.size() - 1, 0.0);
        paddedInput.insert(paddedInput.begin(), padding.begin(), padding.end());
        paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());

        meter.measure([&paddedInput, &kernel, &output]
                      {
                          matlab_like::convolution_valid(paddedInput, kernel, output);
                          return output; });
    };

    BENCHMARK_ADVANCED("(wilczek) Single (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterSingle(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Inner-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Outer-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        auto dividableBy4PaddedInput = std::vector<float>(input);
        dividableBy4PaddedInput.insert(dividableBy4PaddedInput.end(), dividableBy4Padding.begin(), dividableBy4Padding.end());

        wilczek_convolution::FilterInput<float> inputAligned(dividableBy4PaddedInput, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Outer- and Inner-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        auto dividableBy4PaddedInput = std::vector<float>(input);
        dividableBy4PaddedInput.insert(dividableBy4PaddedInput.end(), dividableBy4Padding.begin(), dividableBy4Padding.end());

        wilczek_convolution::FilterInput<float> inputAligned(dividableBy4PaddedInput, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned); });
    };

#ifdef __AVX__
    BENCHMARK_ADVANCED("(wilczek) AVX NoAlign Inner-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterAVX_innerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) AVX NoAlign Outer-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        auto dividableBy4PaddedInput = std::vector<float>(input);
        dividableBy4PaddedInput.insert(dividableBy4PaddedInput.end(), dividableBy4Padding.begin(), dividableBy4Padding.end());

        wilczek_convolution::FilterInput<float, 32> inputAligned(dividableBy4PaddedInput, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterAVX_outerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) AVX NoAlign Outer- and Inner-Loop Vectorization (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        auto dividableBy4PaddedInput = std::vector<float>(input);
        dividableBy4PaddedInput.insert(dividableBy4PaddedInput.end(), dividableBy4Padding.begin(), dividableBy4Padding.end());

        wilczek_convolution::FilterInput<float> inputAligned(dividableBy4PaddedInput, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterAVX_outerInnerLoopVectorization(inputAligned); });
    };
#endif

    BENCHMARK_ADVANCED("(chaowang15) convolve1D (kernel " + std::to_string(kernelLength) + ")")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto kernelSize = kernel.size();

        // "convolve1D" needs the input to be zero padded at the end.
        // To be able to multiply the last sample of "input" with the kernel vector,
        // the input needs further kernel-length - 1 zeroes at the end.
        auto paddedInput = std::vector<float>(input);
        const auto padding = std::vector<float>(kernel.size() - 1, 0.0);
        paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());
        const auto *paddedInputPointer = paddedInput.data();

        const auto *kernelPointer = kernel.data();

        auto *outputPointer = std::addressof(output[0]);

        meter.measure([paddedInputPointer, outputSize, kernelPointer, kernelSize, outputPointer, &output]
                      {
                          chaowang15_convolution::convolve1D(paddedInputPointer, outputPointer, outputSize, kernelPointer, kernelSize);
                          return output; });
    };
}