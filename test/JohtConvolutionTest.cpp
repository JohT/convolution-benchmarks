#include "../source/JohTConvolution.h"
#include "TestVectors.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

SCENARIO("JohT Convolution Implementations")
{
    GIVEN("Input random number vector of size 32")
    {
        const auto &input = randomSize32;

        WHEN("Kernel of size 16 with Daubechies Wavelet filter coefficients)")
        {
            const auto &kernel = waveletFilterCoefficientsDaubechies16;
            const auto convolutionLength = input.size() + kernel.size() - 1;
            auto output = std::vector<float>(convolutionLength, 0.0F);
            const auto &reference = convolutionReferenceResultOfRandomSize32WithDaubechies16;

            THEN("Algorithm 'kernelPerInputValueTransposed' outputs the same result as the reference implementation")
            {
                joht_convolution::kernelPerInputValueTransposed(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposed' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposed(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedInnerLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedOuterLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedOuterLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerAndOuterLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFission' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFission(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelPerInputValueTransposedLoopFission' outputs the same result as the reference implementation")
            {
                joht_convolution::kernelPerInputValueTransposedLoopFission(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFissionIndexArithmetic' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFissionIndexArithmetic(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
        }
    }
}