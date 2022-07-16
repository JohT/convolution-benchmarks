#include "../source/Chaowang15Convolution.h"
#include "TestVectors.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

SCENARIO("Chaowang15 Convolution Algorithms Implementations")
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

            THEN("Algorithm 'convolve1D' outputs the same result as the reference implementation")
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