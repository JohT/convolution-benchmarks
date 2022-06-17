#pragma once
#include <span>

/**
 * @brief My own (JohT) approach to compiler optimize-able convolution implementations.
 * @author JohT
 */
namespace joht_convolution
{

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolution(const std::span<const ValueType> &input, const std::span<const ValueType> &kernel, const std::span<ValueType> &output)
    {
        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto inputIndex = 0; inputIndex < input.size(); ++inputIndex)
        {
            for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
            {
                output[inputIndex + kernelIndex] += input[inputIndex] * kernel[kernelIndex];
            }
        }
    }
}// namespace joht_convolution