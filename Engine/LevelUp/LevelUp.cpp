#include "LevelUp.hpp"

LevelUpManager::LevelUpManager()
{
}

void LevelUpManager::ApplyLevelUp(Character& character)
{
    // This is called by Character::LevelUp()
    // Stat increases are handled per-class in Player subclass
}