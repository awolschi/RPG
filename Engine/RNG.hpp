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
        return static_cast<int>(Engine()() % static_cast<unsigned>(n));
    }

    inline int Percent()
    {
        return Next(100);
    }
}

#endif
