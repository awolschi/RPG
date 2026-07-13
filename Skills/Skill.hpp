#ifndef SKILL_HPP
#define SKILL_HPP

#include <string>

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
};

#endif
