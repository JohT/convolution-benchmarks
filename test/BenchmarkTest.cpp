#include "../source/Din0sConvolution.h"
#include "../source/FIRFilter.h"
#include "../source/RandomVectorGenerator.h"
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

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename SampleType>
void convolution_full(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> &out)
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

//TODO This implementation is not correct.
template<typename SampleType>
void convolution_full_InnerLoopVectorization(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> &out)
{
    auto const inSize = in.size();
    auto const kernelSize = kernel.size();
    auto const outSize = inSize + kernelSize - 1;
    for (auto i(0); i < outSize; ++i)
    {
        auto const jmn = (i >= kernelSize - 1) ? i - (kernelSize - 1) : 0;
        auto const jmx = (i < inSize - 1) ? i : inSize - 1;
        if (((jmx - jmn) >= 4) && ((jmx - jmn) % 4) == 0)
        {
            for (auto j(jmn); j < jmx; j += 4)
            {
                out[i] += (in[j] * kernel[i - j]) +
                          (in[j + 1] * kernel[i - j - 1]) +
                          (in[j + 2] * kernel[i - j - 2]) +
                          (in[j + 3] * kernel[i - j - 3]);
            }
        }
        else
        {
            for (auto j(jmn); j <= jmx; ++j)
            {
                out[i] += (in[j] * kernel[i - j]);
            }
        }
    }
}

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename SampleType>
std::vector<SampleType> convolution_valid(std::vector<SampleType> const &input, std::vector<SampleType> const &kernel, std::vector<SampleType> &out)
{
    const auto inputSize = input.size();
    const auto kernelSize = kernel.size();
    const auto &smallerVector = (inputSize < kernelSize) ? input : kernel;
    const auto &largerVector = (inputSize < kernelSize) ? kernel : input;
    const auto outputSize = std::max(inputSize, kernelSize) - std::min(inputSize, kernelSize) + 1;
    const auto smallerVectorSize = static_cast<int>(smallerVector.size()) - 1;

    for (auto i(0); i < outputSize; ++i)
    {
        for (auto j(smallerVectorSize), k(i); j >= 0; --j)
        {
            out[i] += smallerVector[j] * largerVector[k];
            ++k;
        }
    }
    return out;
}

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
        const auto &input = randomSize32;

        WHEN("Kernel of size 16 with Daubechies Wavelet filter coefficients)")
        {
            const auto &kernel = waveletFilterCoefficientsDaubechies16;
            const auto convolutionLength = input.size() + kernel.size() - 1;
            auto output = std::vector<float>(convolutionLength);
            auto reference = std::vector<float>(convolutionLength);

            // "convolution_valid" is not comparable to "convolution_full".
            // Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
            // THEN("Algorithm 'convolution_valid' outputs the same result as `convolution_full`")
            // {
            //     const auto outSize = std::max(input.size(), kernel.size()) - std::min(input.size(), kernel.size()) + 1;
            //     output = std::vector<float>(outSize);

            //     convolution_full(input, kernel, reference);
            //     convolution_valid(input, kernel, output);
            //     REQUIRE_THAT(output, Catch::Matchers::Equals(reference));
            //}
            //TODO needs to be fixed
            // THEN("Algorithm 'convolution_full_InnerLoopVectorization' outputs the same result as `convolution_full`")
            // {
            //     convolution_full(input, kernel, reference);
            //     convolution_full_InnerLoopVectorization(input, kernel, output);
            //     REQUIRE_THAT(output, Catch::Matchers::Approx(reference).margin(0.001F));
            // }
            THEN("Algorithm 'din0s::convolve' outputs the same result as `convolution_full`")
            {
                convolution_full(input, kernel, reference);
                auto *outPointer = std::addressof(output[0]);
                din0s::convolve(input.data(), kernel.data(), outPointer, input.size(), kernel.size());
                REQUIRE_THAT(output, Catch::Matchers::Equals(reference));
            }
            THEN("Algorithm 'din0s::convolveInputLargerThanKernel' outputs the same result as `convolution_full`")
            {
                convolution_full(input, kernel, reference);
                auto *outPointer = std::to_address(output.begin().base());
                din0s::convolveInputLargerThanKernel(input.data(), kernel.data(), outPointer, input.size(), kernel.size());
                REQUIRE_THAT(output, Catch::Matchers::Equals(reference));
            }
            THEN("Algorithm 'applyFirFilterInnerLoopVectorization' outputs the same result as `applyFirFilterSingle`")
            {
                fir::FilterInput<float> inputAligned(input, kernel);
                fir::FilterInput<float> referenceInputAligned(input, kernel);
                auto outputFir = fir::applyFirFilterInnerLoopVectorization(inputAligned);
                auto referenceFir = fir::applyFirFilterSingle(referenceInputAligned);
                
                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(referenceFir).margin(0.0005F));
            }
            THEN("Algorithm 'applyFirFilterSingle' outputs the same result as `convolution_full`")
            {
                convolution_full(input, kernel, reference);
                
                fir::FilterInput<float> inputAligned(input, kernel);
                convolution_full_InnerLoopVectorization(input, kernel, output);
                auto outputFir = fir::applyFirFilterSingle(inputAligned);
                
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

    BENCHMARK_ADVANCED("convolution_full")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        meter.measure([input, kernel, &out]
                      {
                          convolution_full(input, kernel, out);
                          return out; });
    };

    BENCHMARK_ADVANCED("convolution_full_InnerLoopVectorization")
    (Catch::Benchmark::Chronometer meter)
    {
        auto const outSize = input.size() + kernel.size() - 1;
        std::vector<float> out(outSize);
        meter.measure([input, kernel, &out]
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

        meter.measure([input, kernel, &out]
                      {
                          convolution_valid(input, kernel, out);
                          return out; });
    };

    BENCHMARK_ADVANCED("din0s::convolve")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto inSize = input.size();
        const auto kernelSize = kernel.size();
        const auto outSize = std::max(inSize, kernelSize) - std::min(inSize, kernelSize) + 1;
        std::vector<float> out(outSize);
        auto outIterator = out.begin();
        float *outPointer = &(*outIterator);
        meter.measure([input, kernel, out, outPointer]
                      {
                          din0s::convolve(input.data(), kernel.data(), outPointer, input.size(), kernel.size());
                          return out; });
    };

    BENCHMARK_ADVANCED("din0s::convolveInputLargerThanKernel")
    (Catch::Benchmark::Chronometer meter)
    {
        const auto inSize = input.size();
        const auto kernelSize = kernel.size();
        const auto outSize = std::max(inSize, kernelSize) - std::min(inSize, kernelSize) + 1;
        std::vector<float> out(outSize);
        auto outIterator = out.begin();
        float *outPointer = &(*outIterator);
        meter.measure([input, kernel, out, outPointer]
                      {
                          din0s::convolveInputLargerThanKernel(input.data(), kernel.data(), outPointer, input.size(), kernel.size());
                          return out; });
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