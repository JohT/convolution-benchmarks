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

            const auto inputLength = static_cast<int>(input.size());
            const auto kernelLength = static_cast<int>(kernel.size());

            THEN("Algorithm 'kernelPerInputValueTransposed' outputs the same result as the reference implementation")
            {
                joht_convolution::kernelPerInputValueTransposed(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposed' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposed(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedInnerLoopUnrolled(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedOuterLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedOuterLoopUnrolled(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerAndOuterLoopUnrolled' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFission' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFission(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelPerInputValueTransposedLoopFission' outputs the same result as the reference implementation")
            {
                joht_convolution::kernelPerInputValueTransposedLoopFission(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFissionIndexArithmetic' outputs the same result as the reference implementation")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFissionIndexArithmetic(input.data(), inputLength, kernel.data(),kernelLength, output.data());
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
        }
    }
}