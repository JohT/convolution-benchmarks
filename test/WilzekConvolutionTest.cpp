#include "../source/WilczekConvolution.h"
#include "TestVectors.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

SCENARIO("Wilzek Convolution Implementations")
{
    GIVEN("Input random number vector of size 32")
    {
        const auto &input = randomSize32;

        WHEN("Kernel of size 16 with Daubechies Wavelet filter coefficients)")
        {
            const auto &kernel = waveletFilterCoefficientsDaubechies16;
            const auto convolutionLength = input.size() + kernel.size() - 1;
            const auto &reference = convolutionReferenceResultOfRandomSize32WithDaubechies16;

            auto output = std::vector<float>(convolutionLength, 0.0F);

            // In some cases the input needs to be padded with (here one) zero sample(s) at the end.
            // This way the output length (= input length + filter length - 1)
            // becomes dividable by 4 so that the outer loop can be unrolled by 4.
            const auto dividableBy4PaddingLength = 4 - ((input.size() + kernel.size() - 1) % 4);
            auto padding = std::vector<float>(dividableBy4PaddingLength, 0.0F);

            auto paddedInput = std::vector<float>(input);
            paddedInput.insert(paddedInput.end(), padding.begin(), padding.end());

            auto paddedReference = std::vector<float>(reference);
            paddedReference.insert(paddedReference.end(), padding.begin(), padding.end());

            THEN("Algorithm 'applyFirFilterSingle' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterSingle(inputAligned);

                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference).margin(0.0002F));
            }
            THEN("Algorithm 'applyFirFilterInnerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(paddedInput, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(paddedReference));
            }
            THEN("Algorithm 'applyFirFilterOuterInnerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(paddedInput, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(paddedReference));
            }
        }
    }
}