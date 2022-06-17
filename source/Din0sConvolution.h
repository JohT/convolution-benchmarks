#pragma once
#include <algorithm>
#include <cassert>

/**
 * @brief 1-dimensional convolution implementation
 * Reference: "1-dimensional convolution implementation using C++" https://github.com/din0s/1D-convolution
 * @author: din0s
 */
namespace din0s
{
    /**
     * @brief This method convolutes the two given float arrays. The result is saved in the third array.
     * @author din0s
     */
    inline void convolve(const float *input, const float *kernel, float *output, int inputLen, int kernelLen)
    {
        // Calculate the length of the result
        int abMax = std::max(inputLen, kernelLen);
        int convLen = inputLen + kernelLen - 1;
        for (int n = 0; n < convLen; ++n)
        {
            float prod = 0;

            // Find the minimum amount of iterations needed
            int kMax = std::min(abMax, n);
            for (int k = 0; k <= kMax; ++k)
            {
                // Make sure we're in bounds for both arrays,
                // otherwise there's no overlap between the two.
                if (k < inputLen && n - k < kernelLen)
                {
                    prod += input[k] * kernel[n - k];
                }
            }
            output[n] = prod;
        }
    }
}// namespace din0s