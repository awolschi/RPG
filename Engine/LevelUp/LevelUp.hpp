#ifndef LEVELUP_HPP
#define LEVELUP_HPP

#include "../../Characters/Character.hpp"

class LevelUpManager
{
public:
    LevelUpManager();

    static void ApplyLevelUp(Character& character);
};

#endif
