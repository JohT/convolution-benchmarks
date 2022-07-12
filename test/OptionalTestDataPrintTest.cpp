#include "../source/MatlabLikeConvolution.h"
#include "RandomVectorGenerator.h"
#include "TestVectors.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdio>
#include <fstream>
#include <ostream>
#include <random>
#include <string>

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
    SECTION("Print convolution result values for reference into ConvolutionResultsReference.txt")
    {
        auto &input = randomSize32;
        const auto &kernel = waveletFilterCoefficientsDaubechies16;
        const auto convolutionLength = input.size() + kernel.size() - 1;
        auto reference = std::vector<float>(convolutionLength, 0.0F);
        matlab_like::convolution_full(input, kernel, reference);
        std::ofstream outputFile("ConvolutionResultsReference.txt", std::ios::trunc);
        random_vector_generator::printNumbers(reference, "F,", outputFile);
    }
}