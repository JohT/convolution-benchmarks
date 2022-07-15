#include "../source/JohTConvolution.h"
#include "../source/MatlabLikeConvolution.h"
#include "../source/SongHoAhnConvolution.h"
#include "../source/WilczekConvolution.h"
#include "TestVectors.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

SCENARIO("Convolution Algorithms")
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

            THEN("Algorithm 'kernelPerInputValueTransposed' outputs the same result as `convolution_full`")
            {
                joht_convolution::kernelPerInputValueTransposed(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposed' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposed(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerLoopUnrolled' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposedInnerLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedOuterLoopUnrolled' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposedOuterLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedInnerAndOuterLoopUnrolled' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFission' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFission(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'kernelPerInputValueTransposedLoopFission' outputs the same result as `convolution_full`")
            {
                joht_convolution::kernelPerInputValueTransposedLoopFission(std::span(input), std::span(kernel), std::span(output));
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'inputPerKernelValueTransposedLoopFissionIndexArithmetic' outputs the same result as `convolution_full`")
            {
                joht_convolution::inputPerKernelValueTransposedLoopFissionIndexArithmetic(std::span(input), std::span(kernel), std::span(output));
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

                matlab_like::convolution_valid(paddedInput, kernel, output);
                REQUIRE_THAT(output, Catch::Matchers::Approx(reference));
            }
           THEN("Algorithm 'applyFirFilterSingle' outputs the same result as `convolution_full`")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterSingle(inputAligned);

                //TODO needs quite a bit large margin -> is there something wrong with the implementation?
                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference).margin(0.0002F));
            }
            THEN("Algorithm 'applyFirFilterInnerLoopVectorization' outputs the same result as `convolution_full`")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterLoopVectorization' outputs the same result as `convolution_full`")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'applyFirFilterOuterInnerLoopVectorization' outputs the same result as `convolution_full`")
            {
                wilczek_convolution::FilterInput<float> inputAligned(input, kernel);
                auto outputFir = wilczek_convolution::applyFirFilterOuterInnerLoopVectorization(inputAligned);

                REQUIRE_THAT(outputFir, Catch::Matchers::Approx(reference));
            }
            THEN("Algorithm 'convolve1D' outputs the same result as `convolution_full`")
            {
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