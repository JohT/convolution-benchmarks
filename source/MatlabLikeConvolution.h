#pragma once
#include <vector>

/**
 * @brief Contains simple convolution implementations taken from StackTrace.
 * Reference: "How to perform 1-dimensional "valid" convolution?" - https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
 */
namespace matlab_like
{
    template<typename SampleType>
    void convolution_full(const std::vector<SampleType> &in, const std::vector<SampleType> &kernel, std::vector<SampleType> &out)
    {
        auto const inSize = in.size();
        auto const kernelSize = kernel.size();
        auto const outSize = inSize + kernelSize - 1;
        for (auto i(0); i < outSize; ++i)
        {
            auto const jmn = (i >= kernelSize - 1) ? i - (kernelSize - 1) : 0;
            auto const jmx = (i < inSize - 1) ? i : inSize - 1;
            for (auto j(jmn); j <= jmx; ++j)
            {
                out[i] += (in[j] * kernel[i - j]);
            }
        }
    }

    template<typename T> 
    std::vector<T> convolution_valid(std::vector<T> const &input, std::vector<T> const &kernel, std::vector<T> &output)
    {
        int const inputSize = input.size();
        int const kernelSize = kernel.size();
        std::vector<T> const &shorterVector = (inputSize < kernelSize) ? input : kernel;
        std::vector<T> const &longerVector = (inputSize < kernelSize) ? kernel : input;
        int const outputSize = std::max(inputSize, kernelSize) - std::min(inputSize, kernelSize) + 1;
        for (auto outputIndex = 0; outputIndex < outputSize; ++outputIndex)
        {
            for (int j = (shorterVector.size() - 1), k = outputIndex; j >= 0; --j)
            {
                output[outputIndex] += shorterVector[j] * longerVector[k];
                ++k;
            }
        }
        return output;
    }
}// namespace matlab_like