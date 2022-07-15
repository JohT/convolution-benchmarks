#pragma once
#include "tcb/span.hpp"
#include <vector>

/**
 * @brief Compiler optimize-able convolution implementation experiments.
 * @author JohT
 */
namespace joht_convolution
{
    /**
     * @brief Convolution implementation that multiplies every input value with the whole kernel vector 
     * and sums up the results to the output vector.
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelPerInputValueTransposed(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
        {
            // Makes it obvious for the compiler (especially MSVC) that the factor is constant.
            // Otherwise this message occurs: "1203: Loop body includes on-contiguous accesses into an array".
            const auto inputValue = input[inputIndex];

            for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
            {
                output[inputIndex + kernelIndex] += inputValue * kernel[kernelIndex];
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. 
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * -> This seems to be the fastest approach with CLang compiler vectorization/optimization for small kernels.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposed(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            // Makes it obvious for the compiler (especially MSVC) that the factor is constant.
            // Otherwise this message occurs: "1203: Loop body includes on-contiguous accesses into an array".
            const auto kernelValue = kernel[kernelIndex];

            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                output[inputIndex + kernelIndex] += input[inputIndex] * kernelValue;
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. The inner loop is manually unrolled (4x interleaved).
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *  - The kernel length needs to be dividable by 4 (without remainder).
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedInnerLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; kernelIndex += 4)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                const auto &inputValue = input[inputIndex];
                const auto &outputIndex = inputIndex + kernelIndex;

                output[outputIndex] += inputValue * kernel[kernelIndex];
                output[outputIndex + 1] += inputValue * kernel[kernelIndex + 1];
                output[outputIndex + 2] += inputValue * kernel[kernelIndex + 2];
                output[outputIndex + 3] += inputValue * kernel[kernelIndex + 3];
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. The outer loop is manually unrolled (4x interleaved).
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *  - The input length needs to be dividable by 4 (without remainder).
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedOuterLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; inputIndex += 4)
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
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. The inner and outer loop are manually unrolled (4x interleaved).
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *  - The input and kernel length need to be dividable by 4 (without remainder).
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; kernelIndex += 4)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; inputIndex += 4)
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
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector,
     * stores the results in a temporary buffer and sums up the results to the output vector in a second loop. 
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedLoopFission(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        // TODO The following memory allocation should be done once outside.
        auto scaledInput = std::vector<ValueType>(input.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                scaledInput[inputIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto outputIndex = kernelIndex; outputIndex < inputLength + kernelIndex; ++outputIndex)
            {
                output[outputIndex] += scaledInput[outputIndex - kernelIndex];
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector,
     * stores the results in a temporary buffer and sums up the results to the output vector in a second loop,
     * whereas the index of the output value is calculated (addition). 
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedLoopFissionIndexArithmetic(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());
        const auto outputLength = static_cast<int>(output.size());

        // TODO The following memory allocation should be done once outside.
        auto scaledKernel = std::vector<ValueType>(output.size());

        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                scaledKernel[inputIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto inputIndex = 0; inputIndex < outputLength; ++inputIndex)
            {
                output[inputIndex + kernelIndex] += scaledKernel[inputIndex];
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every input value with the whole kernel vector,
     * stores the (scaled kernel) results in a temporary buffer and sums up the results to the output vector in a second loop. 
     * 
     * <b>Notice:</b>
     *  - The output needs to have a length of inputSize + kernelSize - 1.
     *  - All output values need to be zero initially for a "clean" run. 
     *  - The first kernelSize - 1 values might contain the last values 
     *    of a previous block convolution for a continuos convolution.
     *  - The input does not need to be zero-padded.
     *
     * A drawback of this "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
     * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
     * between loop iterations.
     * 
     * @tparam ValueType 
     * @param input span of input values
     * @param kernel span of kernel values (e.g. filter coefficients)
     * @param output output span
     * @author JohT
     */
    template<typename ValueType>
    void kernelPerInputValueTransposedLoopFission(const tcb::span<const ValueType> &input, const tcb::span<const ValueType> &kernel, const tcb::span<ValueType> &output)
    {
        // Make it obvious for the compiler (e.g. MSVC) that the size of the arrays are constant.
        const auto inputLength = static_cast<int>(input.size());
        const auto kernelLength = static_cast<int>(kernel.size());

        auto scaledKernel = std::vector<ValueType>(kernel.size());

        for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
        {
            for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
            {
                scaledKernel[kernelIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
            {
                output[inputIndex + kernelIndex] += scaledKernel[kernelIndex];
            }
        }
    }

}// namespace joht_convolution