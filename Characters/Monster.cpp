#include "Monster.hpp"

Monster::Monster(const std::string& name, const Stats& baseStats,
                 int experienceReward, int goldReward, bool isBoss)
    : Character(name, baseStats),
      experienceReward(experienceReward),
      goldReward(goldReward),
      isBoss(isBoss)
{
    // Default AI type based on stats
    if (baseStats.intelligence > baseStats.strength)
        aiType = EnemyAIType::Caster;
    else if (baseStats.vitality > 15 && baseStats.defense > 12)
        aiType = EnemyAIType::Defensive;
    else if (baseStats.strength > 20)
        aiType = EnemyAIType::Aggressive;
}

void Monster::AddSpecialDrop(const std::string& name, int chance, int minQty, int maxQty)
{
    specialDrops.push_back({name, chance, minQty, maxQty});
}

void Monster::ScaleStats(int multiplier)
{
    stats.health *= multiplier;
    stats.mana *= multiplier;
    stats.strength *= multiplier;
    stats.vitality *= multiplier;
    stats.intelligence *= multiplier;
    stats.wisdom *= multiplier;
    stats.dexterity *= multiplier;
    stats.defense *= multiplier;
    currentHealth = stats.health;
    currentMana = stats.mana;
    experienceReward *= multiplier;
    goldReward *= multiplier;
}

