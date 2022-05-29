#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <sys/stat.h>

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename T>
std::vector<T> convolution_full(std::vector<T> const &f, std::vector<T> const &g)
{
    auto const nf = f.size();
    auto const ng = g.size();
    auto const n = nf + ng - 1;
    std::vector<T> out(n, T());
    for (auto i(0); i < n; ++i)
    {
        auto const jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
        auto const jmx = (i < nf - 1) ? i : nf - 1;
        for (auto j(jmn); j <= jmx; ++j)
        {
            out[i] += (f[j] * g[i - j]);
        }
    }
    return out;
}

// Reference: https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename T>
std::vector<T> convolution_valid(std::vector<T> const &f, std::vector<T> const &g)
{
    int const nf = f.size();
    int const ng = g.size();
    std::vector<T> const &min_v = (nf < ng) ? f : g;
    std::vector<T> const &max_v = (nf < ng) ? g : f;
    int const n = std::max(nf, ng) - std::min(nf, ng) + 1;
    std::vector<T> out(n, T());
    for (auto i(0); i < n; ++i)
    {
        for (int j(min_v.size() - 1), k(i); j >= 0; --j)
        {
            out[i] += min_v[j] * max_v[k];
            ++k;
        }
    }
    return out;
}

// generate a random vector of size n with values in [-1, 1] with std::random
template<typename T>
std::vector<T> generateVectorWithRealNumbers(int n, T min, T max)
{
    std::vector<T> vectorWithRandomNumbers(n, T());
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_real_distribution<> distribution(min, max);
    for (T &value : vectorWithRandomNumbers)
    {
        value = distribution(gen);
    }
    return vectorWithRandomNumbers;
}

TEST_CASE("Getting started with SIMD Performance Test", "[performance]")
{
    BENCHMARK_ADVANCED("First Steps")
    (Catch::Benchmark::Chronometer meter)
    {
        auto input = generateVectorWithRealNumbers(65536, -1.0, 1.0);
        auto kernel = generateVectorWithRealNumbers(16, 0.0, 1.0);

        meter.measure([input, kernel]
                      { return convolution_full(input, kernel); });
    };
}