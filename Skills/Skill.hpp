#ifndef SKILL_HPP
#define SKILL_HPP

#include <string>
#include <vector>
#include "../Characters/Stats/Stats.hpp"

class Character;
enum class CharacterClass : int;

enum class ElementType
{
    Physical,
    Fire,
    Ice,
    Lightning,
    Arcane,
    Poison,
    Holy
};

inline const char* ElementName(ElementType e)
{
    switch (e)
    {
        case ElementType::Physical:  return "Physical";
        case ElementType::Fire:      return "Fire";
        case ElementType::Ice:       return "Ice";
        case ElementType::Lightning: return "Lightning";
        case ElementType::Arcane:    return "Arcane";
        case ElementType::Poison:    return "Poison";
        case ElementType::Holy:      return "Holy";
    }
    return "Unknown";
}

struct SkillUpgrade
{
    std::string name;
    std::string description;
    int tier;
    int branch;
    bool unlocked = false;

    int damagePercentBonus = 0;
    int manaCostReduction = 0;
    int cooldownReduction = 0;
    int effectDurationBonus = 0;
    int effectDamageBonus = 0;
    int healBonus = 0;
    int defenseBonus = 0;
};

class Skill
{
public:
    Skill(const std::string& name = "", int manaCost = 0, int cooldown = 0, int baseDamage = 0, int requiredLevel = 1)
        : name(name), characterClass(static_cast<CharacterClass>(0)), manaCost(manaCost), cooldown(cooldown), currentCooldown(0), baseDamage(baseDamage), level(1), experience(0), requiredLevel(requiredLevel)
    {
    }

    virtual ~Skill() = default;

    virtual void Use(Character& caster, Character& target);

    void ReduceCooldown();
    bool IsReady() const;
    void ResetCooldown();

    // Skill progression
    void GainXP(int xp);
    void LevelUp();
    static int GetXPToLevel(int currentLevel);

    // Upgrade tree
    virtual void InitializeUpgrades();
    bool CanUnlockUpgrade(int index) const;
    bool UnlockUpgrade(int index);
    int GetTotalDamagePercentBonus() const;
    int GetTotalManaCostReduction() const;
    int GetTotalCooldownReduction() const;
    int GetTotalEffectDurationBonus() const;
    int GetTotalEffectDamageBonus() const;
    int GetTotalHealBonus() const;
    int GetTotalDefenseBonus() const;
    int ApplyDamageBonus(int damage) const;
    int GetEffectiveManaCost() const;
    int GetEffectiveCooldown() const;

    // Damage info for UI display
    virtual std::string GetDamageFormula() const;
    virtual int EstimateDamage(const Stats& stats, int weaponDamage, int elementalBonus) const;

    std::string name;
    std::string description;
    CharacterClass characterClass;
    ElementType element = ElementType::Physical;
    int manaCost;
    int cooldown;
    int currentCooldown;
    int baseDamage;
    int level;
    int experience;
    int requiredLevel;
    int skillPoints = 0;
    std::vector<SkillUpgrade> upgrades;

    // Mastery system (unlocked at skill level 50)
    static constexpr int MASTERY_LEVEL_CAP = 20;
    static constexpr int MASTERY_TREE_BRANCHES = 3;
    static constexpr int MASTERY_NODES_PER_BRANCH = 5;
    int masteryXP = 0;
    int masteryLevel = 0;
    int masteryPoints = 0;
    bool masteryNodes[MASTERY_TREE_BRANCHES][MASTERY_NODES_PER_BRANCH] = {};
    int GetMasteryXPToLevel() const;
    void GainMasteryXP(int xp);
    void MasteryLevelUp();
    bool CanUnlockMasteryNode(int branch, int node) const;
    bool UnlockMasteryNode(int branch, int node);

    // Mastery node bonuses
    int GetMasteryDamageBonus() const;
    int GetMasteryCooldownReduction() const;
    int GetMasteryManaCostReduction() const;
    int GetMasteryXPGainBonus() const;
};

#endif
