//Reference: https://github.com/din0s/1D-convolution
#include <algorithm>

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
}// namespace din0s