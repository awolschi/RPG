#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Character.hpp"
#include <vector>
#include <string>

enum class EnemyAIType
{
    Aggressive,   // Always attacks, high damage focus
    Defensive,    // Defends often, waits for openings
    Caster,       // Prefers skills, manages mana
    Berserker,    // Attacks when low HP, otherwise uses skills
    Balanced,     // Random mix (default)
    Boss          // Smart: heals, uses skills, adapts
};

struct SpecialDrop
{
    std::string itemName;
    int chance;       // percentage 0-100
    int minQty;
    int maxQty;
};

class Monster : public Character
{
public:
    Monster(const std::string& name, const Stats& baseStats,
            int experienceReward, int goldReward, bool isBoss = false);

    int GetExperienceReward() const { return experienceReward; }
    int GetGoldReward() const { return goldReward; }
    bool IsBoss() const { return isBoss; }

    EnemyAIType GetAIType() const { return aiType; }
    void SetAIType(EnemyAIType type) { aiType = type; }

    int GetDropTier() const { return dropTier; }
    void SetDropTier(int tier) { dropTier = tier; }

    const std::vector<SpecialDrop>& GetSpecialDrops() const { return specialDrops; }
    void AddSpecialDrop(const std::string& name, int chance, int minQty = 1, int maxQty = 1);

    void ScaleStats(int multiplier);

protected:
    int experienceReward;
    int goldReward;
    bool isBoss;
    EnemyAIType aiType = EnemyAIType::Balanced;
    int dropTier = 1;
    std::vector<SpecialDrop> specialDrops;
};

#endif
