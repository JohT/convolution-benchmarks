#pragma once

#include <cassert>
#include <vector>

/**
 * @brief Convolution implementations that fully utilize compiler optimizations and auto-vectorization.
 * 
 * The implementations use the "FIR Direct Form Transposed" structure. (L. Wanhammar, DSP Integrated Circuits, 1999)
 *
 * <b>Notice:</b>
 *  - The output needs to have a length of inputSize + kernelSize - 1.
 *  - All output values need to be zero initially for a "clean" run. 
 *  - The first kernelSize - 1 values might contain the last values 
 *    of a previous block convolution for a continuos convolution.
 *  - The input does not need to be zero-padded.
 *
 * A drawback of the "FIR Direct Form Transposed" approach is that intermediate results are written to the output 
 * that need to be read again in the next iteration (to be summed up). This leads to a data dependency
 * between loop iterations.
 *
 * The function "inputPerKernelValueTransposed" 
 * @see https://github.com/JohT/convolution-benchmarks
 * @author JohT
 */
namespace joht_convolution
{
    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. 
     * 
     * This is not only a very simple implementation, it also seems to be the fastest with compiler auto-vectorization/optimization for rather small (e.g. length 16) kernels.
     * It can't compete though with hand-optimized code that uses intrinsics.
     * 
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposed(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            // Makes it obvious for the compiler (especially MSVC) that the factor is constant.
            // Otherwise this message occurs: "1203: Loop body includes on-contiguous accesses into an array".
            const auto kernelValue = kernel[kernelIndex];

            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                // It seems to be beneficial to put the constant factor last when MSVC compile option "/fp:fast" is activated.
                output[inputIndex + kernelIndex] += input[inputIndex] * kernelValue;
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every input value with the whole kernel vector 
     * and sums up the results to the output vector.
     *
     * This is slower than "inputPerKernelValueTransposed" most likely because there are much more
     * multiplications with the same constant value when the kernel value is used as a constant
     * opposed to the input value.
     * 
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void kernelPerInputValueTransposed(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
        {
            // Makes it obvious for the compiler (especially MSVC) that the factor is constant.
            // Otherwise this message occurs: "1203: Loop body includes on-contiguous accesses into an array".
            const auto inputValue = input[inputIndex];

            for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
            {
                // It seems to be beneficial to put the constant factor last when MSVC compile option "/fp:fast" is activated.
                output[inputIndex + kernelIndex] += kernel[kernelIndex] * inputValue;
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every kernel value with the whole input vector 
     * and sums up the results to the output vector. The inner loop is manually unrolled (4x interleaved).
     * 
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedInnerLoopUnrolled(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        // The kernel length needs to be dividable by 4 since its loop is 4 times unrolled/interleaved.
        assert(kernelLength % 4 == 0);

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
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedOuterLoopUnrolled(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        // The input length needs to be dividable by 4 since its loop is 4 times unrolled/interleaved.
        assert(inputLength % 4 == 0);

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
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedInnerAndOuterLoopUnrolled(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        // The input and kernel length needs to be dividable by 4 since its loop is 4 times unrolled/interleaved.
        assert(inputLength % 4 == 0);
        assert(kernelLength % 4 == 0);

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
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedLoopFission(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        // TODO The following memory allocation should be done once outside.
        auto scaledInput = std::vector<ValueType>(inputLength);

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
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void inputPerKernelValueTransposedLoopFissionIndexArithmetic(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        // TODO The following memory allocation should be done once outside.
        auto scaledKernel = std::vector<ValueType>(inputLength + kernelLength - 1);

        for (auto kernelIndex = 0; kernelIndex < kernelLength; ++kernelIndex)
        {
            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                scaledKernel[inputIndex] = input[inputIndex] * kernel[kernelIndex];
            }
            for (auto inputIndex = 0; inputIndex < inputLength; ++inputIndex)
            {
                output[inputIndex + kernelIndex] += scaledKernel[inputIndex];
            }
        }
    }

    /**
     * @brief Convolution implementation that multiplies every input value with the whole kernel vector,
     * stores the (scaled kernel) results in a temporary buffer and sums up the results to the output vector in a second loop. 
     * 
     * @tparam ValueType 
     * @param input pointer to input values
     * @param kernel pointer to kernel values (e.g. filter coefficients)
     * @param output pointer to output values
     * @author JohT
     */
    template<typename ValueType>
    void kernelPerInputValueTransposedLoopFission(const ValueType *const input, const int inputLength, const ValueType *const kernel, const int kernelLength, ValueType *const output)
    {
        auto scaledKernel = std::vector<ValueType>(kernelLength);

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