#pragma once
#include "tcb/span.hpp"

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
    void kernelCentricConvolution(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
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

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * The outer loop provides the kernel index. Since the kernel is smaller and the values (=filter coefficients= are typically constant, 
     * this could be beneficial for caching.
     * -> With CLang vectorization/optimization, this seems to be the fastest way to do this for now (2022-07-05).
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionKernelOuter(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < input.size(); ++inputIndex)
            {
                output[inputIndex + kernelIndex] += input[inputIndex] * kernel[kernelIndex];
            }
        }
    }

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Variant with inner loop unrolled.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionInnerLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto inputIndex = 0; inputIndex < input.size(); ++inputIndex)
        {
            auto inputValue = input[inputIndex];
            for (auto kernelIndex = 0; kernelIndex < kernel.size(); kernelIndex += 4)
            {
                auto outputIndex = inputIndex + kernelIndex;

                output[outputIndex] += inputValue * kernel[kernelIndex];
                output[outputIndex + 1] += inputValue * kernel[kernelIndex + 1];
                output[outputIndex + 2] += inputValue * kernel[kernelIndex + 2];
                output[outputIndex + 3] += inputValue * kernel[kernelIndex + 3];
            }
        }
    }

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Variant with outer loop unrolled.

     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionOuterLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto inputIndex = 0; inputIndex < input.size(); inputIndex += 4)
        {
            for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
            {
                const auto kernelValue = kernel[kernelIndex];
                const auto outputIndex = inputIndex + kernelIndex;

                output[outputIndex] += input[inputIndex] * kernelValue;
                output[outputIndex + 1] += input[inputIndex + 1] * kernelValue;
                output[outputIndex + 2] += input[inputIndex + 2] * kernelValue;
                output[outputIndex + 3] += input[inputIndex + 3] * kernelValue;
            }
        }
    }

    /**
     * @brief Own approach of a convolution implementation with 4-times unrolled inner- and outer loop.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Classic form with inner and outer loop unrolled (4 indices at once).
     * This only works if the input and the kernel have a length that is dividable by 4.
     *
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionInnerAndOuterLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto kernelIndex = 0; kernelIndex < kernel.size(); kernelIndex += 4)
        {
            for (auto inputIndex = 0; inputIndex < input.size(); inputIndex += 4)
            {
                const auto outputIndex = inputIndex + kernelIndex;

                output[outputIndex] += input[inputIndex] * kernel[kernelIndex];

                output[outputIndex + 1] += input[inputIndex] * kernel[kernelIndex + 1] +
                                           input[inputIndex + 1] * kernel[kernelIndex];

                output[outputIndex + 2] += input[inputIndex] * kernel[kernelIndex + 2] +
                                           input[inputIndex + 1] * kernel[kernelIndex + 1] +
                                           input[inputIndex + 2] * kernel[kernelIndex];

                output[outputIndex + 3] += input[inputIndex] * kernel[kernelIndex + 3] +
                                           input[inputIndex + 1] * kernel[kernelIndex + 2] +
                                           input[inputIndex + 2] * kernel[kernelIndex + 1] +
                                           input[inputIndex + 3] * kernel[kernelIndex];

                output[outputIndex + 4] += input[inputIndex + 1] * kernel[kernelIndex + 3] +
                                           input[inputIndex + 2] * kernel[kernelIndex + 2] +
                                           input[inputIndex + 3] * kernel[kernelIndex + 1];

                output[outputIndex + 5] += input[inputIndex + 2] * kernel[kernelIndex + 3] +
                                           input[inputIndex + 3] * kernel[kernelIndex + 2];

                output[outputIndex + 6] += input[inputIndex + 3] * kernel[kernelIndex + 3];
            }
        }
    }

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Variation with inner and outer loop unrolled (4 indices at once).
     * This only works if the input and the kernel have a length that is dividable by 4.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionLoopFissioned(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
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

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Variant with temporary scaled kernel vector.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionTempScaledKernel(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        auto scaledKernel = std::array<ValueType, 16>();

        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto inputIndex = 0; inputIndex < input.size(); ++inputIndex)
        {
            for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
            {
                scaledKernel[kernelIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
            {
                output[inputIndex + kernelIndex] += scaledKernel[kernelIndex];
            }
        }
    }

    /**
     * @brief Own approach of a convolution implementation that multiplies the kernel vector first and uses spans.
     * 
     * The output needs to have a length of inputSize + kernelSize - 1.
     * All output values need to be zero.
     * Variant with temporary scaled kernel vector.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelCentricConvolutionTempScaledOuterLoopKernel(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        auto scaledKernel = std::vector<ValueType>(output.size());

        // Multiply every input sample with the whole kernel vector and add it to the output.
        for (auto kernelIndex = 0; kernelIndex < kernel.size(); ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < input.size(); ++inputIndex)
            {
                scaledKernel[inputIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto inputIndex = 0; inputIndex < output.size(); ++inputIndex)
            {
                output[inputIndex + kernelIndex] += scaledKernel[inputIndex];
            }
        }
    }
}// namespace joht_convolution