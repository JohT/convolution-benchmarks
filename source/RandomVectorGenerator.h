#pragma once
#include <ostream>
#include <random>
#include <iostream>

namespace random_vector_generator
{
    /**
     * @brief Generate a random vector of size n with values in [-1, 1] with std::random
     * 
     * @tparam T numeric type
     * @param n size of the resulting vector
     * @param min minimum value of the random vector
     * @param max maximum value of the random vector
     * @return std::vector<T> 
     */
    template<typename T>
    std::vector<T> randomNumbers(int n, T min, T max)
    {
        std::vector<T> vectorWithRandomNumbers(n, T());
        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_real_distribution<T> distribution(min, max);
        for (T &value : vectorWithRandomNumbers)
        {
            value = distribution(gen);
        }
        return vectorWithRandomNumbers;
    }

    /**
     * @brief Print vector of random numbers with suitable precision
     * 
     * @tparam T numeric value type
     * @param input vector of numeric values
     * @param linePostFix string to append at the end of each value (per line)
     * @param out stream to print to
     */
    template<typename T>
    void printNumbers(std::vector<T> input, const std::string &linePostFix = ",", std::ostream &out = std::cout) {
        out.precision(std::numeric_limits<T>::max_digits10);
        for (const auto &value : input)
        {
            out << std::scientific << value << linePostFix << std::endl;
        }
    }
}// namespace random_vector_generator