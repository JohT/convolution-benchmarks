#include "../source/Din0sConvolution.h"
#include "../source/WilczekConvolution.h"
#include "../source/MatlabLikeConvolution.h"
#include "../source/JohTConvolution.h"
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
#include <span>
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
                joht_convolution::kernelCentricConvolution(std::span(input), std::span(kernel), std::span(output));
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
            THEN("Algorithm 'applyFirFilterInnerLoopVectorization' outputs the same result as `applyFirFilterSingle`")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                wilczek_convolution::FilterInput<float> referenceInputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned);
                auto referenceFir = wilczek_convolution::applyFirFilterSingle(referenceInputAligned);

                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(referenceFir).margin(0.0005F));
            }
            THEN("Algorithm 'applyFirFilterSingle' outputs the same result as `convolution_full`")
            {
                matlab_like::convolution_full(input, kernel, reference);

                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterSingle(inputAligned);

                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference).margin(0.0005F));
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
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        std::span<const float> inputSpan = std::span(input);
        std::span<const float> kernelSpan = std::span(kernel);
        std::span<float> outputSpan = std::span(out);
        meter.measure([&inputSpan, &kernelSpan, &outputSpan, &out]
                      {
                          joht_convolution::kernelCentricConvolution(inputSpan, kernelSpan, outputSpan);
                          return out; });
    };

    BENCHMARK_ADVANCED("(matlab-like) convolution_full")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        meter.measure([input, kernel, &out]
                      {
                          matlab_like::convolution_full(input, kernel, out);
                          return out; });
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

        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);

        meter.measure([paddedInput, kernel, &out]
                      {
                          matlab_like::convolution_valid(paddedInput, kernel, out);
                          return out;
                      });
    };

    BENCHMARK_ADVANCED("(din0s) convolve")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto inputSize = input.size();
        const auto kernelSize = kernel.size();
        const auto outSize = std::max(inputSize, kernelSize) - std::min(inputSize, kernelSize) + 1;
        std::vector<float> out(outSize);
        auto *outPointer = std::to_address(out.begin().base());
        meter.measure([&input, &kernel, outPointer, inputSize, kernelSize, &out]
                      {
                          din0s::convolve(input.data(), kernel.data(), outPointer, inputSize, kernelSize);
                          return out; });
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
}