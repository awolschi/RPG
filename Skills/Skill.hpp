#ifndef SKILL_HPP
#define SKILL_HPP

#include <string>
#include <vector>

class Character;

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
        : name(name), manaCost(manaCost), cooldown(cooldown), currentCooldown(0), baseDamage(baseDamage), level(1), experience(0), requiredLevel(requiredLevel)
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

    std::string name;
    std::string description;
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
};

#endif
