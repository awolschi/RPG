#ifndef LEVELUP_HPP
#define LEVELUP_HPP

#include "../../Characters/Character.hpp"

class LevelUpManager
{
public:
    LevelUpManager();

    static int CalculateRequiredXP(int level);
    static void ApplyLevelUp(Character& character);
};

#endif
