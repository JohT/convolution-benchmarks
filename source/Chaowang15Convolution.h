#pragma once

/**
 * @brief 1d convolution algorithm by Song Ho Ahn
 * Reference: "Fast Image Convolution in C++" https://github.com/chaowang15/fast-image-convolution-cpp
 * @author chaowang15
 */
namespace chaowang15_convolution
{
    /**
     * @brief 1D convolution
     * We assume input and kernel signal start from t=0.
     * Reference: https://github.com/chaowang15/fast-image-convolution-cpp/blob/master/src/convolution.cpp
     * @param input input signal
     * @param kernel kernel signal
     * @param output output signal
     * @param size input signal size
     * @param kernel_size kernel signal size
     * @param stride stride
     * @author chaowang15
     */
    template<typename SampleType>
    bool convolve1D(const SampleType *in, SampleType *out, int dataSize, const SampleType *kernel, int kernelSize)
    {
        int i, j, k;

        // check validity of params
        if (!in || !out || !kernel)
        {
            return false;
        }
        if (dataSize <= 0 || kernelSize <= 0)
        {
            return false;
        }

        // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
        for (i = kernelSize - 1; i < dataSize; ++i)
        {
            out[i] = 0;// init to 0 before accumulate

            for (j = i, k = 0; k < kernelSize; --j, ++k)
            {
                out[i] += in[j] * kernel[k];
            }
        }

        // convolution from out[0] to out[kernelSize-2]
        for (i = 0; i < kernelSize - 1; ++i)
        {
            out[i] = 0;// init to 0 before sum

            for (j = i, k = 0; j >= 0; --j, ++k)
            {
                out[i] += in[j] * kernel[k];
            }
        }

        return true;
    }
}// namespace chaowang15_convolution