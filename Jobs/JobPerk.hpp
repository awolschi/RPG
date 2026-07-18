#ifndef JOB_PERK_HPP
#define JOB_PERK_HPP

#include <string>
#include <vector>

enum class JobPerkType
{
    SellValueBonus,     // +X% sell value for resources
    DoubleChance,       // X% chance to collect double resources
    RareFindChance,     // X% chance to find a rare resource
    CombatDefense,      // +X DEF in combat after working this job
    CombatDamage,       // +X% damage in combat after working this job
    CombatHealth,       // +X max HP in combat after working this job
    MasteryBonus        // Special mastery perk at level 10
};

struct JobPerk
{
    std::string name;
    std::string description;
    JobPerkType type;
    int value;          // The numeric value of the perk (percentage or flat)
    int requiredLevel;  // Job level needed to unlock
    int branch;         // 0 or 1 (two branches)
    int tier;           // 1, 2, or 3
    bool unlocked = false;

    JobPerk(const std::string& name, const std::string& desc, JobPerkType type,
            int value, int requiredLevel, int branch, int tier)
        : name(name), description(desc), type(type), value(value),
          requiredLevel(requiredLevel), branch(branch), tier(tier), unlocked(false) {}
};

#endif
