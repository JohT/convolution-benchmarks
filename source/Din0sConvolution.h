//Reference: https://github.com/din0s/1D-convolution
#include <algorithm>
#include <cassert>

namespace din0s
{
    /**
     * This method convolutes the two given float arrays.
     * The result is saved in the third array.
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
    /**
     * This method convolutes the two given float arrays.
     * The result is saved in the third array.
     * This method assumes, that the input is larger than the kernel (filter coefficients) and fails otherwise.
     */
    inline void convolveInputLargerThanKernel(const float *input, const float *kernel, float *output, int inputLength, int kernelLength)
    {
        assert(inputLength > kernelLength);
        // Calculate the length of the result
        int convolutionLength = inputLength + kernelLength - 1;
        for (int outputIndex = 0; outputIndex < convolutionLength; ++outputIndex)
        {
            float prod = 0.0F;

            // Find the minimum amount of iterations needed
            int kernelMax = std::min(inputLength, outputIndex);
            for (int kernelIndex = 0; kernelIndex <= kernelMax; ++kernelIndex)
            {
                // Make sure we're in bounds for both arrays,
                // otherwise there's no overlap between the two.
                if (kernelIndex < inputLength && outputIndex - kernelIndex < kernelLength)
                {
                    prod += input[kernelIndex] * kernel[outputIndex - kernelIndex];
                }
            }
            output[outputIndex] = prod;
        }
    }
}// namespace din0s