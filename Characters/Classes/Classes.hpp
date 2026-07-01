#ifndef CLASSES_H
#define CLASSES_H

#include "../Stats/Stats.hpp"

enum class CharacterClass
{
    Warrior,
    Priest,
    Mage,
    Archer
};

struct ClassData
{
    Stats startingStats;
    Stats levelUpGain;
};

class ClassDatabase
{
public:
    static ClassData Get(CharacterClass characterClass);
};

#endif