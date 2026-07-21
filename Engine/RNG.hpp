#ifndef RNG_HPP
#define RNG_HPP

#include <random>

namespace RNG
{
    inline std::mt19937& Engine()
    {
        static std::mt19937 gen(std::random_device{}());
        return gen;
    }

    inline int Next(int n)
    {
        if (n <= 0) return 0;
        return std::uniform_int_distribution<int>(0, n - 1)(Engine());
    }

    inline int Percent()
    {
        return Next(100);
    }
}

#endif
