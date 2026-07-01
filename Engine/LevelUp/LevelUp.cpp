#include "LevelUp.hpp"

LevelUpManager::LevelUpManager()
{
}

int LevelUpManager::CalculateRequiredXP(int level)
{
    // XP requirement increases with level
    // Formula: 100 + (level - 1) * 50
    return 100 + (level - 1) * 50;
}

void LevelUpManager::ApplyLevelUp(Character& character)
{
    // This is called by Character::LevelUp()
    // Stat increases are handled per-class in Player subclass
}