#include "LevelUp.hpp"

LevelUpManager::LevelUpManager()
{
}

int LevelUpManager::CalculateRequiredXP(int level)
{
    return 100 + (level - 1) * 100 + (level - 1) * (level - 1) * 15;
}

void LevelUpManager::ApplyLevelUp(Character& character)
{
    // This is called by Character::LevelUp()
    // Stat increases are handled per-class in Player subclass
}