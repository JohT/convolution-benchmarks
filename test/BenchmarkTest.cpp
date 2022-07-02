#include "../source/Din0sConvolution.h"
#include "../source/JohTConvolution.h"
#include "../source/MatlabLikeConvolution.h"
#include "../source/SongHoAhnConvolution.h"
#include "../source/WilczekConvolution.h"
#include "RandomVectorGenerator.h"
#include "TestVectors.h"

#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <sys/stat.h>

/**
 * @brief The following test case will only run on demand and will print out test data.
 */
TEST_CASE("Print test data", "[.][print]")
{
    SECTION("Print random numbers with exponential notation into RandomNumbers.txt")
    {
        auto input = random_vector_generator::randomNumbers(16384, -1.0F, 1.0F);
        std::ofstream outputFile("RandomNumbers.txt", std::ios::trunc);
        random_vector_generator::printNumbers(input, "F,", outputFile);
    }
}

SCENARIO("Convolution Algorithms")
{
    GIVEN("Input random number vector of size 32")
    {
        auto &input = randomSize32;

        WHEN("Kernel of size 16 with Daubechies Wavelet filter coefficients)")
        {
            const auto &kernel = waveletFilterCoefficientsDaubechies16;
            const auto convolutionLength = input.size() + kernel.size() - 1;
            auto output = std::vector<float>(convolutionLength, 0.0F);
            auto reference = std::vector<float>(convolutionLength, 0.0F);

            THEN("Algorithm 'kernelCentricConvolution' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolution(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionKernelOuter' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionKernelOuter(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionInnerLoopUnrolled' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionInnerLoopUnrolled(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionOuterLoopUnrolled' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionOuterLoopUnrolled(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionInnerAndOuterLoopUnrolled' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionInnerAndOuterLoopUnrolled(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionTempScaledKernel' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionTempScaledKernel(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelCentricConvolutionTempScaledOuterLoopKernel' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                joht_convolution::kernelCentricConvolutionTempScaledOuterLoopKernel(tcb::span(input), tcb::span(kernel), tcb::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            //Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
            THEN("Algorithm 'convolution_valid' outputs the same result as `convolution_full`")
            {
                // "valid" means that the input needs to be padded with (kernel size - 1) zeroes
                // at the beginning and end of the data input vector before the convolution.
                auto paddedInput = std::vector<float>(input);
                auto padding = std::vector<float>(kernel.size() - 1, 0.0);
                paddedInput.insert(paddedInput.begin(), padding.begin(), padding.end());
                paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());

                matlab_like::convolution_full(input, kernel, reference);
                matlab_like::convolution_valid(paddedInput, kernel, output);
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'din0s::convolve' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);
                auto *outPointer = std::addressof(output[0]);
                din0s::convolve(input.data(), kernel.data(), outPointer, input.size(), kernel.size());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterSingle' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterSingle(inputAligned);

                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference).margin(0.0002F));
            }
            THEN("Algorithm 'applyFirFilterInnerLoopVectorization' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterLoopVectorization' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterInnerLoopVectorization' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'convolve1D' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                // "convolve1D" needs the input to be zero padded at the end.
                // To be able to multiply the last sample of "input" with the kernel vector,
                // the input needs further kernel-length - 1 zeroes at the end.
                auto paddedInput = std::vector<float>(input);
                auto padding = std::vector<float>(kernel.size() - 1, 0.0);
                paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());

                auto *outputPointer = std::addressof(output[0]);
                chaowang15_convolution::convolve1D(paddedInput.data(), outputPointer, paddedInput.size(), kernel.data(), kernel.size());

                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
        }
    }
}
TEST_CASE("Benchmark Convolution Algorithms", "[performance]")
{
    auto input = random_vector_generator::randomNumbers(16384, -1.0F, 1.0F);
    auto kernel = random_vector_generator::randomNumbers(16, 0.0F, 1.0F);

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

    BENCHMARK_ADVANCED("(din0s) convolve")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto inputSize = static_cast<int>(input.size());
        const auto inputPointer = input.data();

        const auto kernelSize = static_cast<int>(kernel.size());
        const auto kernelPointer = kernel.data();

        const auto outSize = std::max(inputSize, kernelSize) - std::min(inputSize, kernelSize) + 1;
        std::vector<float> output(outSize);
        auto *outputPointer = std::to_address(output.begin().base());

        meter.measure([inputPointer, kernelPointer, outputPointer, inputSize, kernelSize, &output]
                      {
                          din0s::convolve(inputPointer, kernelPointer, outputPointer, inputSize, kernelSize);
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