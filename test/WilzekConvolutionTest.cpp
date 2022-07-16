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
            auto output = std::vector<float>(convolutionLength, 0.0F);
            const auto &reference = convolutionReferenceResultOfRandomSize32WithDaubechies16;

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
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterInnerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }

#ifdef __AVX__
            THEN("Algorithm 'applyFirFilterAVX_innerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterAVX_innerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterAVX_outerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float, 32> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterAVX_outerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterAVX_outerInnerLoopVectorization' outputs the same result as the reference implementation")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterAVX_outerInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
#endif
        }
    }
}