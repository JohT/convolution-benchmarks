#include "../source/JohTConvolution.h"
#include "../source/MatlabLikeConvolution.h"
#include "../source/SongHoAhnConvolution.h"
#include "../source/WilczekConvolution.h"
#include "RandomVectorGenerator.h"
#include "TestVectors.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdio>
#include <memory>
#include <string>

TEST_CASE("Benchmark Convolution Algorithms", "[performance]")
{
    const auto input = random_vector_generator::randomNumbers(16384, -1.0F, 1.0F);
    const auto kernel = random_vector_generator::randomNumbers(16, 0.0F, 1.0F);

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolution")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolution(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionKernelOuter")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionKernelOuter(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionInnerLoopUnrolled")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionInnerLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionOuterLoopUnrolled")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionOuterLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionInnerAndOuterLoopUnrolled")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionInnerAndOuterLoopUnrolled(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionTempScaledKernel")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionTempScaledKernel(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(JohT) kernelCentricConvolutionTempScaledOuterLoopKernel")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        const tcb::span<const float> inputSpan = tcb::span(input);
        const tcb::span<const float> kernelSpan = tcb::span(kernel);
        const tcb::span<float> outputSpan = tcb::span(output);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &output]
                      {
                          joht_convolution::kernelCentricConvolutionTempScaledOuterLoopKernel(inputSpan, kernelSpan, outputSpan);
                          return output; });
    };

    BENCHMARK_ADVANCED("(matlab-like) convolution_full")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        meter.measure([&input, &kernel, &output]
                      {
                          matlab_like::convolution_full(input, kernel, output);
                          return output; });
    };

    BENCHMARK_ADVANCED("(matlab-like) convolution_valid")
    (Catch::Benchmark::Chronometer meter)
    {
        // "valid" means that the input needs to be padded with (kernel size - 1) zeroes
        // at the beginning and end of the data input vector before the convolution.
        auto paddedInput = std::vector<float>(input);
        auto padding = std::vector<float>(kernel.size() - 1, 0.0);
        paddedInput.insert(paddedInput.begin(), padding.begin(), padding.end());
        paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());

        const auto outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);

        meter.measure([&paddedInput, &kernel, &output]
                      {
                          matlab_like::convolution_valid(paddedInput, kernel, output);
                          return output; });
    };

    BENCHMARK_ADVANCED("(wilczek) Single")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterSingle(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Inner-Loop Vectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Outer-Loop Vectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(wilczek) Outer- and Inner-Loop Vectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        wilczek_convolution::FilterInput<float> inputAligned(input, kernel);

        meter.measure([&inputAligned]
                      { return wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned); });
    };

    BENCHMARK_ADVANCED("(chaowang15) convolve1D")
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

        auto const outputSize = input.size() + kernel.size() - 1;
        std::vector<float> output(outputSize);
        auto *outputPointer = std::addressof(output[0]);

        meter.measure([paddedInputPointer, outputSize, kernelPointer, kernelSize, outputPointer, &output]
                      {
                          chaowang15_convolution::convolve1D(paddedInputPointer, outputPointer, outputSize, kernelPointer, kernelSize);
                          return output; });
    };
}