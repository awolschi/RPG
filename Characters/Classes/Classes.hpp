#ifndef CLASSES_H
#define CLASSES_H

#include "../Stats/Stats.hpp"
#include "../../Items/Item.hpp"
#include <vector>

enum class CharacterClass
{
    Warrior,
    Priest,
    Mage,
    Archer,
    Merchant
};

struct ClassData
{
    Stats startingStats;
    Stats levelUpGain;
    std::vector<WeaponType> allowedWeaponTypes;
    std::vector<ArmorType> allowedArmorTypes;
};

class ClassDatabase
{
public:
    static ClassData Get(CharacterClass characterClass);
};

#endif